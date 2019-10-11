/*
 * EPS_CMD.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Hoopoe3n
 */

#ifndef EPS_CMD_H_
#define EPS_CMD_H_

#include "../../EPS.h"

#include "../../COMM/splTypes.h"
#include "../../COMM/GSC.h"
#include "../../Global/sizes.h"
#include "../../Global/Global.h"
#include "../../Global/TLM_management.h"

void cmd_upload_volt_logic(Ack_type* type, ERR_type* err, TC_spl cmd);
void cmd_heater_temp(Ack_type* type, ERR_type* err, TC_spl cmd);
void cmd_upload_volt_COMM(Ack_type* type, ERR_type* err, TC_spl cmd);
void cmd_SHUT_ADCS(Ack_type* type, ERR_type* err, TC_spl cmd);
void cmd_SHUT_CAM(Ack_type* type, ERR_type* err, TC_spl cmd);
void cmd_allow_ADCS(Ack_type* type, ERR_type* err, TC_spl cmd);
void cmd_allow_CAM(Ack_type* type, ERR_type* err, TC_spl cmd);


#endif /* EPS_CMD_H_ */
