#include <satellite-subsystems/IsisAntS.h>


#include "SubSystemModules/Communication/SubsystemCommands/TRXVU_Commands.h"
#include "SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.h"
#include "SubSystemModules/Communication/SubsystemCommands/FS_Commands.h"
#include "SubSystemModules/Communication/SubsystemCommands/EPS_Commands.h"

#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "TLM_management.h"
#include <stdio.h>
#include "CommandDictionary.h"

int trxvu_command_router(sat_packet_t* cmd)
{
	int err = 0;
	sat_packet_t delayed_cmd = { 0 };
	//TODO: finish 'trxvu_command_router'
	switch (cmd->cmd_subtype)
	{
	case DUMP_SUBTYPE:
		err = CMD_StartDump(cmd);
		break;

	case ABORT_DUMP_SUBTYPE:
		err = CMD_SendDumpAbortRequest(cmd);
		break;

	case FORCE_ABORT_DUMP_SUBTYPE:
		err = CMD_ForceDumpAbort(cmd);
		break;

	case MUTE_TRXVU:
		err = CMD_MuteTRXVU(cmd);
		break;

	case UNMUTE_TRXVU:
		err = CMD_UnMuteTRXVU(cmd);
		break;

	case GET_BAUD_RATE:
		err = CMD_GetBaudRate(cmd);
		break;

	case GET_BEACON_INTERVAL:
		err = CMD_GetBeaconInterval(cmd);
		break;

	case SET_BEACON_INTERVAL:
		err = CMD_SetBeaconInterval(cmd);
		break;

				return 0;
		}
	

int eps_command_router(sat_packet_t *cmd)
{//need to know witch subtype should call witch command
	return 0;
}

int telemetry_command_router(sat_packet_t *cmd)
{//need to know witch subtype should call witch command
	return 0;
}

int managment_command_router(sat_packet_t *cmd)
{
	//TODO: finish 'managment_command_router'
	int err = 0;
	switch ((management_subtypes_t)cmd->cmd_subtype)
	{

	case SOFT_RESET_SUBTYPE:
		CMD_ResetComponent(reset_software);
		break;

	case HARD_RESET_SUBTYPE:
		CMD_ResetComponent(reset_hardware);
		break;


	}
	return err;
}

int filesystem_command_router(sat_packet_t *cmd)
{//need to know witch subtype should call witch command
	return 0;
}
