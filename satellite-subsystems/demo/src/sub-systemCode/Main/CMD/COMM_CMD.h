/*
 * COMM_CMD.h
 *
 *  Created on: Jun 22, 2019
 *      Author: Hoopoe3n
 */

#ifndef COMM_CMD_H_
#define COMM_CMD_H_

#include "../../COMM/GSC.h"
#include "../../Global/Global.h"

void cmd_error(Ack_type* type, ERR_type* err);

void cmd_mute(Ack_type* type, ERR_type* err, TC_spl cmd);

void cmd_unmute(Ack_type* type, ERR_type* err);

void cmd_active_trans(Ack_type* type, ERR_type* err, TC_spl cmd);

void cmd_shut_trans(Ack_type* type, ERR_type* err);

void cmd_change_trans_rssi(Ack_type* type, ERR_type* err, TC_spl cmd);

void cmd_aprs_dump(Ack_type* type, ERR_type* err);

void cmd_stop_dump(Ack_type* type, ERR_type* err);

void cmd_time_frequency(Ack_type* type, ERR_type* err, TC_spl cmd);

#endif /* COMM_CMD_H_ */
