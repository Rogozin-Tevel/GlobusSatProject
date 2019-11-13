#include <string.h>
#include <hal/errors.h>

#include "TRXVU.h"
#include "ActUponCommand.h"
#include "CommandDictionary.h"
#include "AckHandler.h"

int ActUponCommand(sat_packet_t *cmd)
{
	switch ((spl_command_type)cmd->cmd_type)
	{
	case trxvu_cmd_type:
		err = trxvu_command_router(cmd);
		break;
	}
	return err;
}


