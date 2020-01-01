#include <string.h>

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>

#include "GlobalStandards.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "TRXVU_Commands.h"

int CMD_StartDump(sat_packet_t* cmd) {
	return DumpTelemetry(cmd);
}

int CMD_SendDumpAbortRequest(sat_packet_t *cmd) {
	SendDumpAbortRequest();
	return 0; // What's the returned value
}

int CMD_ForceDumpAbort(sat_packet_t *cmd) {
	AbortDump();
	return CheckDumpAbort();
}

int CMD_MuteTRXVU(sat_packet_t *cmd) {
	return muteTRXVU((time_unix*) ((void*) cmd->data));
}

int CMD_UnMuteTRXVU(sat_packet_t *cmd) {
	UnMuteTRXVU();
	return GetMuteFlag();
}

int CMD_GetBaudRate(sat_packet_t *cmd) {
	return GetTrxvuBitrate((ISIStrxvuBitrateStatus*) ((void*) cmd->data)); // Check parameter, I'm not sure about it
}

int CMD_SetBeaconCycleTime(sat_packet_t *cmd) {
	return FRAM_write(cmd->data, BEACON_BITRATE_CYCLE_ADDR, cmd->length);
}

int CMD_GetBeaconInterval(sat_packet_t *cmd) {
	return FRAM_read(cmd->data, BEACON_INTERVAL_TIME_ADDR, cmd->length);
}

int CMD_SetBeaconInterval(sat_packet_t *cmd)
{
	time_unix *pTime = (time_unix *)cmd->data;
	return UpdateBeaconInterval(*pTime); // Used function instead of writing into FRAM as advised
}

int CMD_SetBaudRate(sat_packet_t *cmd) {
	ISIStrxvuBitrate bitrate = 0;
	int err = IsisTrxvu_tcSetAx25Bitrate(ISIS_TRXVU_I2C_BUS_INDEX, &bitrate);
	if(err != 0)
	{
		return err;
	}

	return TransmitDataAsSPL_Packet(cmd, &bitrate, sizeof(ISIStrxvuBitrate))
}

int CMD_GetTxUptime(sat_packet_t *cmd) {
	unsigned int uptime = 0;
	int err = IsisTrxvu_tcGetUptime(ISIS_TRXVU_I2C_BUS_INDEX, &uptime);
	if (err != 0)
	{
		return err;
	}

	return TransmitDataAsSPL_Packet(cmd, &uptime, sizeof(unsigned int))
}

int CMD_GetRxUptime(sat_packet_t *cmd) {
	unsigned int uptime = 0;
	int err = IsisTrxvu_rcGetUptime(ISIS_TRXVU_I2C_BUS_INDEX, &uptime);
	if (err != 0)
	{
		return err;
	}

	return TransmitDataAsSPL_Packet(cmd, &uptime, sizeof(unsigned int))
}

int CMD_GetNumOfDelayedCommands(sat_packet_t *cmd) {
	return GetDelayedCommandBufferCount();
}

int CMD_GetNumOfOnlineCommands(sat_packet_t *cmd) {
	unsigned short frameCount = 0;
	int err = IsisTrxvu_rcGetFrameCount(ISIS_TRXVU_I2C_BUS_INDEX, &frameCount) 

	if (err != 0)
	{
		return err;
	}
	return TransmitDataAsSPL_Packet(cmd, &frameCount, sizeof(frameCount));
}

int CMD_DeleteDelyedCmdByID(sat_packet_t *cmd) {
	return DeleteDelayedCommandByIndex(cmd->data[0]);
}

int CMD_DeleteAllDelyedBuffer(sat_packet_t *cmd) {
	return DeleteDelayedBuffer();
}

int CMD_AntSetArmStatus(sat_packet_t *cmd) {
	return IsisAntS_setArmStatus(ISIS_TRXVU_I2C_BUS_INDEX, cmd->data[0], cmd->data[1]);
}

int CMD_AntGetArmStatus(sat_packet_t *cmd) {
	ISISantsSide ant_side = cmd->data[0];
	ISISantsStatus status = 0;

	int err = IsisAntS_getStatusData(ISIS_TRXVU_I2C_BUS_INDEX, ant_side, &status);
	if (err != 0)
	{
		return err;
	}
	
	return TransmitDataAsSPL_Packet(cmd, &status, sizeof(ISISantsStatus));
}

int CMD_AntGetUptime(sat_packet_t *cmd) {
	unsigned int up_time = 0;
	int err = IsisAntS_getUptime(ISIS_TRXVU_I2C_BUS_INDEX, cmd->data[0], &up_time);
	if (err != 0) {
		return err;
	}
	return TransmitDataAsSPL_Packet(cmd, &up_time, sizeof(int));
}

int CMD_AntCancelDeployment(sat_packet_t *cmd) {
	return IsisAntS_cancelDeployment(ISIS_TRXVU_I2C_BUS_INDEX, cmd->data[0]);
}

