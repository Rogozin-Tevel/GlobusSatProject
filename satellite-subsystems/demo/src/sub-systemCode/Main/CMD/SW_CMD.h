/*
 * SW_CMD.h
 *
 *  Created on: Jun 22, 2019
 *      Author: Hoopoe3n
 */

#ifndef SW_CMD_H_
#define SW_CMD_H_

#include "../../COMM/GSC.h"
#include "../../Global/Global.h"

void cmd_reset_APRS_list(Ack_type* type, ERR_type* err);

void cmd_reset_delayed_command_list(Ack_type* type, ERR_type* err);

void cmd_reset_FRAM(Ack_type* type, ERR_type* err, TC_spl cmd);

#endif /* SW_CMD_H_ */
