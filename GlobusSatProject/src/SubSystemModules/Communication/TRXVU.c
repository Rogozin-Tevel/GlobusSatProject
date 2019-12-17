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
#define SIZE_RXFRAME	200
#define SIZE_TXFRAME	235


Boolean 		g_mute_flag = MUTE_OFF;				// mute flag - is the mute enabled
time_unix 		g_mute_end_time = 0;				// time at which the mute will end
time_unix 		g_prev_beacon_time = 0;				// the time at which the previous beacon occured
time_unix 		g_beacon_interval_time = 0;			// seconds between each beacon
unsigned char	g_current_beacon_period = 0;		// marks the current beacon cycle(how many were transmitted before change in baud)
unsigned char 	g_beacon_change_baud_period = 0;	// every 'g_beacon_change_baud_period' beacon will be in 1200Bps and not 9600Bps

xQueueHandle     xDumpQueue = NULL;
xSemaphoreHandle xDumpLock = NULL;
xTaskHandle      xDumpHandle = NULL;	 //task handle for dump task
xSemaphoreHandle xIsTransmitting = NULL; // mutex on transmission.

void InitSemaphores()
{
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
			//TODO: log error
		}
		BeaconLogic();

		if (command_found != err)
			return err;

		return command_succsess;
}

int GetNumberOfFramesInBuffer() {
	return 0;
}

Boolean CheckTransmitionAllowed() {
	return FALSE;
}

void FinishDump(dump_arguments_t *task_args,unsigned char *buffer, ack_subtype_t acktype,
		unsigned char *err, unsigned int size) {
}

void AbortDump()
{
}

void SendDumpAbortRequest() {
}

Boolean CheckDumpAbort() {
	return FALSE;
}

void DumpTask(void *args) {
}

int DumpTelemetry(sat_packet_t *cmd) {
	return 0;
}

//Sets the bitrate to 1200 every third beacon and to 9600 otherwise
int BeaconSetBitrate() {
	return 0;
}

void BeaconLogic()
{

	if(!CheckTransmitionAllowed()){
			return;
		}

		int err = 0;
		if (!CheckExecutionTime(g_prev_beacon_time, g_beacon_interval_time)) {
			return;
		}

		WOD_Telemetry_t wod = { 0 };
		GetCurrentWODTelemetry(&wod);

		sat_packet_t cmd = { 0 };
		err = AssmbleCommand((unsigned char*)&wod, sizeof(wod), trxvu_cmd_type,
				BEACON_SUBTYPE, 0xFFFFFFFF, &cmd);
		if (0 != err) {
			return;
		}

		Time_getUnixEpoch(&g_prev_beacon_time);

		BeaconSetBitrate();

		TransmitSplPacket(&cmd, NULL);
		IsisTrxvu_tcSetAx25Bitrate(ISIS_TRXVU_I2C_BUS_INDEX, trxvu_bitrate_9600);
}

int muteTRXVU(time_unix duration)
{
	if (duration > MAX_MUTE_TIME) {
		return -2;
	}
	time_unix curr_tick_time = 0;
	Time_getUnixEpoch(&curr_tick_time);

	g_mute_end_time = curr_tick_time + duration;
	g_mute_flag = MUTE_ON;

	return 0;
}

void UnMuteTRXVU() {
	g_mute_end_time = 0;
    g_mute_flag = MUTE_OFF;
}

Boolean GetMuteFlag() {

	if (g_mute_flag)
		return MUTE_ON;
	return MUTE_OFF;
	//return g_mute_flag;
}

Boolean CheckForMuteEnd() {

	time_unix curr_tick_time = 0;
	Time_getUnixEpoch(&curr_tick_time);
	return (curr_tick_time > g_mute_end_time);
}

int GetTrxvuBitrate(ISIStrxvuBitrateStatus *bitrate) {
	return 0;
}

int TransmitDataAsSPL_Packet(sat_packet_t *cmd, unsigned char *data,
		unsigned int length)
{
	return 0;
}

int TransmitSplPacket(sat_packet_t *packet, int *avalFrames) {

	if (!CheckTransmitionAllowed()) {
		return -1;
	}

	if (NULL == packet) {
		return E_NOT_INITIALIZED;
	}

	int err = 0;
	unsigned int data_length = packet->length + sizeof(packet->length)
			+ sizeof(packet->cmd_subtype) + sizeof(packet->cmd_type)
			+ sizeof(packet->ID);

	if (xSemaphoreTake(xIsTransmitting,SECONDS_TO_TICKS(1)) != pdTRUE) {
		return E_GET_SEMAPHORE_FAILED;
	}
	err = IsisTrxvu_tcSendAX25DefClSign(ISIS_TRXVU_I2C_BUS_INDEX,
			(unsigned char*) packet, data_length, (unsigned char*) &avalFrames);

	xSemaphoreGive(xIsTransmitting);

	return err;
}

int UpdateBeaconBaudCycle(unsigned char cycle)
{
	return 0;
}

int UpdateBeaconInterval(time_unix intrvl) {
	return 0;
}
