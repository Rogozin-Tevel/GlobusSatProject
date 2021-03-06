#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Timing/Time.h>
#include <hal/errors.h>
#include "SatCommandHandler.h"
#include <satellite-subsystems/IsisAntS.h>
#include <satellite-subsystems/IsisTRXVU.h>
#include <SubSystemModules/Communication/SatCommandHandler.h>
#include <stdlib.h>
#include <string.h>

#include "GlobalStandards.h"
#include "TRXVU.h"
#include "AckHandler.h"
#include "ActUponCommand.h"
#include "SatCommandHandler.h"
#include "TLM_management.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "SubSystemModules/Housekeeping/TelemetryCollector.h"
#ifdef TESTING_TRXVU_FRAME_LENGTH
#include <hal/Utility/util.h>
#endif

xQueueHandle     xDumpQueue = NULL;
xSemaphoreHandle xDumpLock = NULL;
xTaskHandle      xDumpHandle = NULL;	 //task handle for dump task

void InitSemaphores()
{
	if(NULL == xDumpQueue)
		xDumpQueue = xQueueCreate(1, sizeof(Boolean));
	if(xDumpLock == NULL)
		vSemaphoreCreateBinary(xDumpLock);
}

int InitTrxvu() 
{
	int err = E_NO_SS_ERR;

	/*
	 * Inititalize the TRXVU
	 */
	{
		ISIStrxvuI2CAddress address = 
		{
			I2C_TRXVU_RC_ADDR, I2C_TRXVU_TC_ADDR
		};

		ISIStrxvuFrameLengths maxFrameLengths = 
		{
			SIZE_TXFRAME, SIZE_RXFRAME
		};

		ISIStrxvuBitrate defaultBitrate = trxvu_bitrate_9600;

		err = IsisTrxvu_initialize(&address, &maxFrameLengths, &defaultBitrate, 1);
		if (err != E_NO_SS_ERR)
		{
			return err;
		}

		const portTickType ticksToDelay = 100; // # of ticks
		vTaskDelay(ticksToDelay);
	}

	/*
	 * Inititalize the antenna
	 */
	{
		ISISantsI2Caddress address =
		{
			ANTS_I2C_SIDE_A_ADDR, ANTS_I2C_SIDE_B_ADDR
		};

		err = IsisAntS_initialize(&address, 1);
		if (err != E_NO_SS_ERR)
		{
			return err;
		}

		const portTickType ticksToDelay = 100; // # of ticks
		vTaskDelay(ticksToDelay);
	}

	/*
	 * Initialize the semaphores.
	 * TODO: Document...?
	 */
	InitSemaphores();

	/*
	 * Load the beacon's bitrate cycle
	 */
	err = FRAM_read(&g_beacon_change_baud_period, BEACON_BITRATE_CYCLE_ADDR, BEACON_BITRATE_CYCLE_SIZE);
	if (err)
	{
		g_beacon_change_baud_period = DEFALUT_BEACON_BITRATE_CYCLE;
	}

	/*
	 * Load the beacon's interval time
	 */
	err = FRAM_read(&g_beacon_interval_time, BEACON_INTERVAL_TIME_ADDR, BEACON_INTERVAL_TIME_SIZE);
	if (err)
	{
		g_beacon_interval_time = DEFAULT_BEACON_INTERVAL_TIME;
	}

	return E_NO_SS_ERR;
}

int TRX_Logic() 
{
	    int err = 0;
		int frame_count = GetNumberOfFramesInBuffer();
		sat_packet_t cmd = { 0 };

		if (frame_count > 0) {
			err = GetOnlineCommand(&cmd);
			ResetGroundCommWDT();
			SendAckPacket(ACK_RECEIVE_COMM, &cmd, NULL, 0);

		} else if (GetDelayedCommandBufferCount() > 0) {
			err = GetDelayedCommand(&cmd);
		}
		if (command_found == err) {
			err = ActUponCommand(&cmd);
		}
		BeaconLogic();

		if (command_found != err)
			return err;

		return command_succsess;
}

void FinishDump(dump_arguments_t *task_args,unsigned char *buffer, ack_subtype_t acktype,
		unsigned char *err, unsigned int size)
{	
	SendAckPacket(acktype, task_args->cmd, err, size);
	if(NULL != buffer)
	{
		free(buffer);
	}
	if (NULL != task_args) 
	{
		free(task_args);
	}
	if (NULL != xDumpLock) 
	{
		xSemaphoreGive(xDumpLock);
	}
	if (xDumpHandle != NULL) 
	{
		vTaskDelete(xDumpHandle);
	}
}

void AbortDump(){
	FinishDump(NULL,NULL,ACK_DUMP_ABORT,NULL,0);
}

void SendDumpAbortRequest() {
	if (eTaskGetState(xDumpHandle) == eDeleted) {
		return;
	}
	Boolean queue_msg = TRUE;
	int err = xQueueSend(xDumpQueue, &queue_msg, SECONDS_TO_TICKS(1));
	
	if (err != 0)
	{
		if (xDumpHandle != NULL) {
			vTaskDelete(xDumpHandle);
		}
		if (xDumpLock != NULL) {
			xSemaphoreGive(xDumpLock);
		}
	}
}

Boolean CheckDumpAbort() 
{
	portBASE_TYPE err;
	Boolean queue_msg;
	err = xQueueReceive(xDumpQueue, &queue_msg, SECONDS_TO_TICKS(1));

	if (err == pdPASS) {
		return queue_msg;
	}
	return FALSE;
}

void DumpTask(void *args)
{
	if (NULL == args) {
		FinishDump(NULL, NULL, ACK_DUMP_ABORT, NULL, 0);
		return;
	}
	dump_arguments_t *task_args = (dump_arguments_t *) args;
	sat_packet_t dump_tlm = { 0 };

	int err = 0;
	int availFrames = 0;
	unsigned int num_of_elements = 0;
	unsigned int size_of_element = 0;

	unsigned char *buffer = NULL;
	char filename[MAX_F_FILE_NAME_SIZE] = { 0 };

	err = GetTelemetryFilenameByType((tlm_type) task_args->dump_type,
			filename);
	if (0 != err) {
		FinishDump(task_args, buffer, ACK_DUMP_ABORT, (unsigned char*) &err,sizeof(err));
		return;
	}

	SendAckPacket(ACK_DUMP_START, task_args->cmd,
			(unsigned char*) &num_of_elements, sizeof(num_of_elements));

	for (unsigned int i = 0; i < num_of_elements; i++) {

		if (CheckDumpAbort() || CheckTransmitionAllowed()) {
			FinishDump(task_args, buffer, ACK_DUMP_ABORT, NULL, 0);
		}
		if (0 == availFrames)
			vTaskDelay(10);

		AssembleCommand((unsigned char*)buffer, size_of_element,
				(char) DUMP_SUBTYPE, (char) (task_args->dump_type),
				task_args->cmd->ID, &dump_tlm);

		//TODO: finish dump

		TransmitSplPacket(&dump_tlm, &availFrames);
	}
	FinishDump(task_args, buffer, ACK_DUMP_FINISHED, NULL, 0);
}

int DumpTelemetry(sat_packet_t *cmd)
{
if (NULL == cmd) {
		return -1;
	}

	dump_arguments_t *dmp_pckt = malloc(sizeof(*dmp_pckt));
	unsigned int offset = 0;

	dmp_pckt->cmd = cmd;

	memcpy(&dmp_pckt->dump_type, cmd->data, sizeof(dmp_pckt->dump_type));
	offset += sizeof(dmp_pckt->dump_type);

	memcpy(&dmp_pckt->t_start, cmd->data + offset, sizeof(dmp_pckt->t_start));
	offset += sizeof(dmp_pckt->t_start);

	memcpy(&dmp_pckt->t_end, cmd->data + offset, sizeof(dmp_pckt->t_end));

	if (xSemaphoreTake(xDumpLock,SECONDS_TO_TICKS(1)) != pdTRUE) {
		return E_GET_SEMAPHORE_FAILED;
	}
	xTaskCreate(DumpTask, (const signed char* const )"DumpTask", 2000,
			(void* )dmp_pckt, configMAX_PRIORITIES - 2, xDumpHandle);
	return 0;
}