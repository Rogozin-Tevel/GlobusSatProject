#include <string.h>

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>

#include "GlobalStandards.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "TRXVU_Commands.h"

int CMD_StartDump(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SendDumpAbortRequest(sat_packet_t *cmd)
{
	return 0;
}

int CMD_ForceDumpAbort(sat_packet_t *cmd)
{
	return 0;
}


int CMD_MuteTRXVU(sat_packet_t *cmd)
{
	return 0;
}

int CMD_UnMuteTRXVU(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetBaudRate(sat_packet_t *cmd)
{
	return 0;
}


int CMD_SetBeaconCycleTime(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetBeaconInterval(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SetBeaconInterval(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SetBaudRate(sat_packet_t *cmd)
{
	return 0;
}


int CMD_GetTxUptime(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetRxUptime(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetNumOfDelayedCommands(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetNumOfOnlineCommands(sat_packet_t *cmd)
{
	return 0;
}

int CMD_DeleteDelyedCmdByID(sat_packet_t *cmd)
{
	return 0;
}

int CMD_DeleteAllDelyedBuffer(sat_packet_t *cmd)
{
	return 0;
}

int CMD_AntSetArmStatus(sat_packet_t *cmd)
{
	return 0;
}

int CMD_AntGetArmStatus(sat_packet_t *cmd)
{
	return 0;
}

int CMD_AntGetUptime(sat_packet_t *cmd)
{
	return 0;
}

int CMD_AntCancelDeployment(sat_packet_t *cmd)
{
	return 0;
}

