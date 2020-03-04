#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "GlobalStandards.h"

#include <hal/Timing/Time.h>

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>
#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif


#include <hcc/api_fat.h>
#include <hal/Drivers/I2C.h>
#include <stdlib.h>
#include <string.h>
#include <hal/errors.h>
#include "TLM_management.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "Maintanence_Commands.h"

int CMD_GenericI2C(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	unsigned char slaveAddr = 0;
	unsigned int size = 0;
	unsigned char *i2c_data = malloc(size);

	memcpy(&slaveAddr, cmd->data, sizeof(slaveAddr));
	memcpy(&size,cmd->data + sizeof(slaveAddr), sizeof(size));

	unsigned int offset = sizeof(slaveAddr) + sizeof(size);
	int err = I2C_write((unsigned int)slaveAddr,cmd->data + offset, cmd->length);
	I2C_read((unsigned int)slaveAddr, i2c_data, size);

	return err;
}

int CMD_FRAM_ReadAndTransmitt(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	unsigned int addr = 0;
	unsigned int size = 0;

	memcpy(&addr, cmd->data, sizeof(addr));
	memcpy(&size, cmd->data + sizeof(addr),sizeof(size));

	unsigned char *read_data = malloc(size);
	if(NULL == read_data){
		return E_MEM_ALLOC;
	}

	int err = FRAM_read(read_data, addr, size);
	if (err != 0){
		return err;
	}

	TransmitDataAsSPL_Packet(cmd, read_data, size);

	free(read_data);
	return err;
}

int CMD_FRAM_WriteAndTransmitt(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	unsigned int addr = 0;
	unsigned int length = cmd->length;
	unsigned char *data = cmd->data;

	memcpy(&addr, cmd->data, sizeof(addr));

	int err = FRAM_write(data + sizeof(addr), addr, length - sizeof(addr));
	if (err != 0){
		return err;
	}

	err = FRAM_read(data, addr, length - sizeof(addr));
	if (err != 0){
		return err;
	}

	return TransmitDataAsSPL_Packet(cmd, data, length);
}

int CMD_FRAM_Start(sat_packet_t *cmd)
{
	return FRAM_start();
}

int CMD_FRAM_Stop(sat_packet_t *cmd)
{
	return FRAM_stop();
}

int CMD_FRAM_GetDeviceID(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}
	char* deviceID = 0;
	FRAM_getDeviceID(deviceID);

	return TransmitDataAsSPL_Packet(cmd, deviceID, sizeof(deviceID));
}

int CMD_UpdateSatTime(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}
	unsigned char* data = 0;
	time_unix current_time;

	memcpy(&current_time, cmd->data, sizeof(current_time));
	err = Time_setUnixEpoch(set_time);
	TransmitDataAsSPL_Packet(cmd, (unsigned char*)&set_time, sizeof(set_time));

	return err;
}

int CMD_GetSatTime(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}
	unsigned char* data = 0;
	time_unix curr_time = 0;

	int err = Time_getUnixEpoch(&curr_time);
	if (err != 0)
	{
		return err;
	}

	return TransmitDataAsSPL_Packet(cmd, data, sizeof(time_unix));
}

int CMD_GetSatUptime(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}
	unsigned char* data = 0;

	time_unix uptime = Time_getUptimeSeconds();
	
	memcpy(data, &uptime, sizeof(int));

	return TransmitDataAsSPL_Packet(cmd, data, sizeof(int));
}

int CMD_SoftTRXVU_ComponenetReset(sat_packet_t *cmd)
{
	int err = 0;
	err = IsisTrxvu_componentSoftReset(,0x00)// TODO insert const sent in whatsapp group + change hex values to def names
	if (err != 0)	// If error on first reset, stop and return error as to not overwrite the first one
	{	return err	}
	IsisTrxvu_componentSoftReset(0x01)// TODO insert const sent in whatsapp group + change hex values to def names
	return err;
}

int CMD_HardTRXVU_ComponenetReset(sat_packet_t *cmd)
{
	int err = 0;
	err = IsisTrxvu_componentHardReset(0x00) // TODO insert const sent in whatsapp group + change hex values to def names
	if (err != 0)	// If error on first reset, stop and return error as to not overwrite the first one
	{	return err	}
	IsisTrxvu_componentHardReset(0x01) // TODO insert const sent in whatsapp group+ change hex values to def names
	return err;
}

int CMD_AntennaDeploy(sat_packet_t *cmd)
{
	err = IsisAntS_setArmStatus(ISIS_TRXVU_I2C_BUS_INDEX , isisants_sideA, isisants_arm);
	if(err != E_NO_SS_ERR ){
		return err;
	}
	err = IsisAntS_setArmStatus(ISIS_TRXVU_I2C_BUS_INDEX , isisants_sideB, isisants_arm);
	if(err != E_NO_SS_ERR ){
		return err;
	}

	err = IsisAntS_autoDeployment(ISIS_TRXVU_I2C_BUS_INDEX, isisants_sideA, ANTENNA_DEPLOYMENT_TIMEOUT);
	if(err != E_NO_SS_ERR ){
		return err;
	}

	err = IsisAntS_autoDeployment(ISIS_TRXVU_I2C_BUS_INDEX, isisants_sideB, ANTENNA_DEPLOYMENT_TIMEOUT);
	return err;
}

int CMD_ResetComponent(reset_type_t rst_type)
{
	int err = 0;
	Boolean8bit reset_flag = TRUE_8BIT;

	switch (rst_type)
	{
	case reset_software:
		FRAM_write(&reset_flag, RESET_CMD_FLAG_ADDR, RESET_CMD_FLAG_SIZE);
		SendAnonymosAck(ACK_SOFT_RESET);
		vTaskDelay(10);
		restart();
		break;

	case reset_hardware:
		SendAnonymosAck(ACK_HARD_RESET);
		FRAM_write(&reset_flag, RESET_CMD_FLAG_ADDR, RESET_CMD_FLAG_SIZE);
		vTaskDelay(10);
		//TODO: obc hard reset
		break;

	case reset_eps:
		SendAnonymosAck(ACK_EPS_RESET);
		FRAM_write(&reset_flag, RESET_CMD_FLAG_ADDR, RESET_CMD_FLAG_SIZE);
		vTaskDelay(10);

		ieps_statcmd_t ieps_cmd;
		err = IsisEPS_hardReset(EPS_I2C_BUS_INDEX, &ieps_cmd);
		break;

	case reset_trxvu_hard:
		SendAnonymosAck(ACK_TRXVU_HARD_RESET);
		err = IsisTrxvu_hardReset(ISIS_TRXVU_I2C_BUS_INDEX);
		vTaskDelay(100);
		break;

	case reset_trxvu_soft:
		SendAnonymosAck(ACK_TRXVU_SOFT_RESET);
		err = IsisTrxvu_softReset(ISIS_TRXVU_I2C_BUS_INDEX);
		vTaskDelay(100);
		break;

	case reset_filesystem:
		DeInitializeFS();
		vTaskDelay(10);
		err = (unsigned int) InitializeFS(FALSE);
		vTaskDelay(10);
		SendAckPacket(ACK_FS_RESET, NULL, (unsigned char*) &err, sizeof(err));
		break;

	case reset_ant_SideA:
		err = IsisAntS_reset(ISIS_TRXVU_I2C_BUS_INDEX, isisants_sideA);
		SendAckPacket(ACK_ANTS_RESET, NULL, (unsigned char*) &err, sizeof(err));
		break;

	case reset_ant_SideB:
		err = IsisAntS_reset(ISIS_TRXVU_I2C_BUS_INDEX, isisants_sideB);
		SendAckPacket(ACK_ANTS_RESET, NULL, (unsigned char*) &err, sizeof(err));
		break;

	default:
		SendAnonymosAck(ACK_UNKNOWN_SUBTYPE);
		break;
	}
}
