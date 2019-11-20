#include <string.h>
#include <hal/errors.h>

#include "TRXVU.h"
#include "ActUponCommand.h"
#include "CommandDictionary.h"
#include "AckHandler.h"

int ActUponCommand(sat_packet_t *cmd)  // Made By Blank
{

	switch ((spl_command_type)cmd->cmd_type)

	int err = 0;

	switch(cmd->cmd_type)

	{
	case trxvu_cmd_type:
		err = trxvu_command_router(cmd);
		break;

	}

	case eps_cmd_type:
		err = eps_command_router(cmd);
		break;
	case telemetry_cmd_type:
		err = telemetry_command_router(cmd);
		break;
	case filesystem_cmd_type:
		err = filesystem_command_router(cmd);
		break;
	case managment_cmd_type:
		err = managment_command_router(cmd);
		break;
	default:
		// Error! Unknown cmd type. No appropriate error for invalid cmd type
		break;
	}


	return err;
}


