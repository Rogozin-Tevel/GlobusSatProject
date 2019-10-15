/*
 * commands.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Hoopoe3n
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>
#include <satellite-subsystems/GomEPS.h>
#include <satellite-subsystems/SCS_Gecko/gecko_use_cases.h>
#include <satellite-subsystems/SCS_Gecko/gecko_driver.h>
#include <satellite-subsystems/cspaceADCS.h>
#include <satellite-subsystems/cspaceADCS_types.h>
#include <hal/boolean.h>
#include <hal/errors.h>

#include "../COMM/GSC.h"
#include "../Global/sizes.h"
#include "../Global/Global.h"
#include "../Global/TLM_management.h"

//ADCS
#define ADCS_INDEX 1
#define COMMAND_LIST_SIZE 20

/**
 *@
 */
int init_command();
int add_command(TC_spl command);
int get_command(TC_spl* command);

void act_upon_command(TC_spl decode);


void AUC_COMM(TC_spl decode);

void AUC_general(TC_spl decode);

void AUC_payload(TC_spl decode);

void AUC_EPS(TC_spl decode);

void AUC_ADCS(TC_spl decode);

void AUC_GS(TC_spl decode);

void AUC_SW(TC_spl decode);

void AUC_special_operation(TC_spl decode);

#endif /* COMMANDS_H_ */
