/*
 * SW_CMD.c
 *
 *  Created on: Jun 22, 2019
 *      Author: Hoopoe3n
 */
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "SW_CMD.h"

#include "../../TRXVU.h"
#include "../../EPS.h"
#include "../../Main/Main_Init.h"
#include "../../COMM/APRS.h"
#include "../../COMM/DelayedCommand_list.h"

void cmd_reset_delayed_command_list(Ack_type* type, ERR_type* err)
{
	*type = ACK_RESET_DELAYED_CMD;
	reset_delayCommand(FALSE);
	*err = ERR_SUCCESS;
}
void cmd_reset_APRS_list(Ack_type* type, ERR_type* err)
{
	*type = ACK_RESET_APRS_LIST;
	reset_APRS_list(FALSE);
	*err = ERR_SUCCESS;
}
void cmd_reset_FRAM(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_FRAM_RESET;
	if (cmd.length != 1)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	*err = ERR_SUCCESS;
	subSystem_indx sub = (subSystem_indx)cmd.data[0];
	switch (sub)
	{
		case EPS:
			reset_FRAM_EPS();
			break;
		case TRXVU:
			reset_FRAM_TRXVU();
			break;
		case ADCS:
			break;
		case OBC:
			reset_FRAM_MAIN();
			break;
		case CAMMERA:
			break;
		case everything:
			reset_FRAM_MAIN();
			reset_EPS_voltages();
			reset_FRAM_TRXVU();
			//reset ADCS, cammera
			//grecful reset
			break;
		default:
			*err = ERR_PARAMETERS;
			break;
	}
}
