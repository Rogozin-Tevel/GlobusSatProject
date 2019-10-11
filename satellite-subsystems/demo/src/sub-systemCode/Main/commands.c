/*
 * commands.c
 *
 *  Created on: Dec 5, 2018
 *      Author: Hoopoe3n
 */
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <at91/utility/exithandler.h>
#include <at91/commons.h>
#include <at91/utility/trace.h>
#include <at91/peripherals/cp15/cp15.h>

#include <hal/Utility/util.h>
#include <hal/Timing/WatchDogTimer.h>
#include <hal/Timing/Time.h>
#include <hal/Drivers/I2C.h>
#include <hal/Drivers/LED.h>
#include <hal/boolean.h>
#include <hal/errors.h>

#include <hal/Storage/FRAM.h>

#include <string.h>

#include "commands.h"

#include "CMD/EPS_CMD.h"
#include "CMD/General_CMD.h"
#include "CMD/COMM_CMD.h"
#include "CMD/SW_CMD.h"
#include "CMD/payload_CMD.h"

#ifdef TESTING
#include "CMD/test_CMD.h"
#endif

#include "../COMM/splTypes.h"
#include "../COMM/DelayedCommand_list.h"
#include "../Global/Global.h"
#include "../TRXVU.h"
#include "../Ants.h"
#include "../ADCS.h"
#include "HouseKeeping.h"
#include "../EPS.h"

#define create_task(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask) xTaskCreate( (pvTaskCode) , (pcName) , (usStackDepth) , (pvParameters), (uxPriority), (pxCreatedTask) ); vTaskDelay(10);

/*TODO:
 * 1. finish all commands function
 * 2. remove comments from AUC to execute commands
 * 3. change the way commands pass throw tasks
 */

xSemaphoreHandle xCTE = NULL;

TC_spl command_to_execute[COMMAND_LIST_SIZE];
int place_in_list = 0;


void copy_command(TC_spl source, TC_spl* to)
{
	// copy command from source to to
	to->id = source.id;
	to->type = source.type;
	to->subType = source.subType;
	to->length = source.length;
	to->time = source.time;
	memcpy(to->data, source.data, (int)source.length);
}
void reset_command(TC_spl *command)
{
	// set to 0 all parameters of command
	memset((void*)command, 0, SIZE_OF_COMMAND);
}

//todo: change name to more sugnifficent
int init_command()
{
	if (xCTE != NULL)
		return 1;
	// 1. create semaphore
	vSemaphoreCreateBinary(xCTE);
	// 2. set place in list to 0
	place_in_list = 0;
	// 3. allocate memory for data in command
	int i;
	for (i = 0; i < COMMAND_LIST_SIZE; i++)
	{
		reset_command(&command_to_execute[i]);
	}
	return 0;
}
int add_command(TC_spl command)
{
	portBASE_TYPE error;
	// 1. try to take semaphore
	if (xSemaphoreTake(xCTE, MAX_DELAY) == pdTRUE)
	{
		// 2. if queue full
		if (place_in_list == COMMAND_LIST_SIZE)
		{
			error = xSemaphoreGive(xCTE);
			check_portBASE_TYPE("could not return xCTE in add_command", error);
			return 1;
		}
		// 3. copy command to list
		copy_command(command, &command_to_execute[place_in_list]);
		// 4. moving forward current place in command queue
		place_in_list++;
		// 5. return semaphore
		error = xSemaphoreGive(xCTE);
		check_portBASE_TYPE("cold not return xCTE in add_command", error);
	}

	return 0;
}
int get_command(TC_spl* command)
{
	portBASE_TYPE error;
	// 1. try to take semaphore
	if (xSemaphoreTake(xCTE, MAX_DELAY) == pdTRUE)
	{
		// 2. check if there's commands in list
		if (place_in_list == 0)
		{
			error = xSemaphoreGive(xCTE);
			check_portBASE_TYPE("get_command, xSemaphoreGive(xCTE)", error);
			return 1;
		}
		// 3. copy first in command to TC_spl* command
		copy_command(command_to_execute[0], command);
		int i;
		// 4. move every command forward in queue
		for (i = 0; i < place_in_list - 1; i++)
		{
			reset_command(&command_to_execute[i]);
			copy_command(command_to_execute[i + 1], &command_to_execute[i]);
		}
		// 5. reset the last place for command
		reset_command(&command_to_execute[i]);
		place_in_list--;
		// 6. return semaphore
		error = xSemaphoreGive(xCTE);
		check_portBASE_TYPE("get_command, xSemaphoreGive(xCTE)", error);
	}
	return 0;
}
int check_number_commands()
{
	return place_in_list;
}

void act_upon_command(TC_spl decode)
{
	//later use in ACK
	switch (decode.type)
	{
	case (COMM_T):
		AUC_COMM(decode);
		break;
	case (GENERAL_T):
		AUC_general(decode);
		break;
	case (PAYLOAD_T):
		AUC_payload(decode);
		break;
	case (EPS_T):
		AUC_EPS(decode);
		break;
	case (TC_ADCS_T):
		AUC_ADCS(decode);
		break;
	case (GENERALLY_SPEAKING_T):
		AUC_GS(decode);
		break;
	case (SOFTWARE_T):
		AUC_SW(decode);
		break;
	case (SPECIAL_OPERATIONS_T):
		AUC_special_operation(decode);
		break;
	default:
		printf("wrong type: %d\n", decode.type);
		break;
	}
}


void AUC_COMM(TC_spl decode)
{
	Ack_type type;
	ERR_type err;
	switch (decode.subType)
	{
	case (MUTE_ST):
		cmd_mute(&type, &err, decode);
		break;
	case (UNMUTE_ST):
		cmd_unmute(&type, &err);
		break;
	case (ACTIVATE_TRANS_ST):
		cmd_active_trans(&type, &err, decode);
		return;
		break;
	case (SHUT_TRANS_ST):
		cmd_shut_trans(&type, &err);
		break;
	case (CHANGE_TRANS_RSSI_ST):
		cmd_change_trans_rssi(&type, &err, decode);
		break;
	case (APRS_DUMP_ST):
		cmd_aprs_dump(&type, &err);
		break;
	case (STOP_DUMP_ST):
	   cmd_stop_dump(&type, &err);
		break;
	case (TIME_FREQUENCY_ST):
		cmd_time_frequency(&type, &err, decode);
		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

void AUC_general(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	case (SOFT_RESET_ST):
		cmd_soft_reset_cmponent(&type, &err, decode);
		break;
	case (HARD_RESET_ST):
		cmd_hard_reset_cmponent(&type, &err, decode);
		break;
	case (RESET_SAT_ST):
		cmd_reset_satellite(&type, &err);
		break;
	case (GRACEFUL_RESET_ST):
		cmd_gracefull_reset_satellite(&type, &err);
		break;
	case (UPLOAD_TIME_ST):
		cmd_upload_time(&type, &err, decode);
		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

void AUC_payload(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	case (SEND_PIC_CHUNCK_ST):
		break;
	case (UPDATE_STN_PARAM_ST):
		return;
		break;
	case GET_IMG_DATA_BASE_ST:
		break;
	case RESET_DATA_BASE_ST:
		break;
	case DELETE_PIC_ST:
		return;
		break;
	case UPD_DEF_DUR_ST:
		break;
	case OFF_CAM_ST:
		break;
	case ON_CAM_ST:
		break;
	case MOV_IMG_CAM_OBS_ST:
		return;
		break;
	case TAKE_IMG_DEF_VAL_ST:
		return;
		break;
	case TAKE_IMG_ST:
		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

void AUC_EPS(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	case (UPD_LOGIC_VOLT_ST):
		cmd_upload_volt_logic(&type, &err, decode);
		break;
	case (CHANGE_HEATER_TMP_ST):
		cmd_heater_temp(&type, &err, decode);
		break;
	case (UPD_COMM_VOLTAGE):
		cmd_upload_volt_COMM(&type, &err, decode);
		break;
	case (ALLOW_ADCS_ST):
		cmd_allow_ADCS(&type, &err, decode);
		break;
	case (SHUT_ADCS_ST):
		cmd_SHUT_ADCS(&type, &err, decode);
		break;
	case (ALLOW_CAM_ST):
		cmd_allow_CAM(&type, &err, decode);
		break;
	case (SHUT_CAM_ST):
		cmd_SHUT_CAM(&type, &err, decode);
		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

void AUC_ADCS(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

void AUC_GS(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	case (GENERIC_I2C_ST):
		cmd_generic_I2C(&type, &err, decode);
		break;
	case (DUMP_ST):
		cmd_dump(decode);
		return;
		break;
	case (DELETE_PACKETS_ST):
		cmd_delete_TM(&type, &err, decode);
		break;
	case (RESET_FILE_ST):
		cmd_reset_file(&type, &err, decode);
		break;
	case (RESTSRT_FS_ST):
		break;
	case (DUMMY_FUNC_ST):
		cmd_dummy_func(&type, &err);
		break;
	case (REDEPLOY):

		break;
	case (ARM_DISARM):
		cmd_ARM_DIARM(&type, &err, decode);
		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

void AUC_SW(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	case (RESET_APRS_LIST_ST):
		cmd_reset_APRS_list(&type, &err);
		break;
	case (RESET_DELAYED_CM_LIST_ST):
		cmd_reset_delayed_command_list(&type, &err);
		break;
	case (RESET_FRAM_ST):
		cmd_reset_FRAM(&type, &err, decode);
		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

void AUC_special_operation(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	case DELETE_UNF_CUF_ST:
		break;
	case UPLOAD_CUF_ST:
		break;
	case CON_UNF_CUF_ST:
		break;
	case PAUSE_UP_CUF_ST:
		break;
	case EXECUTE_CUF:
		break;
	case REVERT_CUF:
		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}

#ifdef TESTING
void AUC_test(TC_spl decode)
{
	Ack_type type;
	ERR_type err;

	switch (decode.subType)
	{
	case IMAGE_DUMP_ST:

		break;
	default:
		cmd_error(&type, &err);
		break;
	}
	//Builds ACK
#ifndef NOT_USE_ACK_HK
	save_ACK(type, err, decode.id);
#endif
}
#endif
