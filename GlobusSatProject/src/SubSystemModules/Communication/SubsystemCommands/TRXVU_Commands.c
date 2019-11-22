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
	return GetTrxvuBitrate((ISIStrxvuBitrateStatus*) ((void*) cmd->data)); // Check parameter
}

int CMD_SetBeaconCycleTime(sat_packet_t *cmd) {
	return FRAM_write(cmd->data, BEACON_BITRATE_CYCLE_ADDR, cmd->length);
}

int CMD_GetBeaconInterval(sat_packet_t *cmd) {
	return FRAM_read(cmd->data, BEACON_INTERVAL_TIME_ADDR, cmd->length);
}

int CMD_SetBeaconInterval(sat_packet_t *cmd) // Used function instead of writing into FRAM as advised
{
	return UpdateBeaconInterval((time_unix*) ((void*) cmd->data));
}

int CMD_SetBaudRate(sat_packet_t *cmd) {
	return IsisTrxvu_tcSetAx25Bitrate(/*unsigned char index, ISIStrxvuBitrate bitrate*/);  // Where do i get the params from?
}

int CMD_GetTxUptime(sat_packet_t *cmd) {
	return IsisTrxvu_tcGetUptime(/*unsigned char index, unsigned int *uptime*/);  // Where do i get the params from?
}

int CMD_GetRxUptime(sat_packet_t *cmd) {
	return 0;
}

int CMD_GetNumOfDelayedCommands(sat_packet_t *cmd) {
	return 0;
}

int CMD_GetNumOfOnlineCommands(sat_packet_t *cmd) {
	return 0;
}

int CMD_DeleteDelyedCmdByID(sat_packet_t *cmd) {
	return 0;
}

int CMD_DeleteAllDelyedBuffer(sat_packet_t *cmd) {
	return 0;
}

int CMD_AntSetArmStatus(sat_packet_t *cmd) {
	return 0;
}

int CMD_AntGetArmStatus(sat_packet_t *cmd) {
	return 0;
}

int CMD_AntGetUptime(sat_packet_t *cmd) {
	return 0;
}

int CMD_AntCancelDeployment(sat_packet_t *cmd) {
	return 0;
}

