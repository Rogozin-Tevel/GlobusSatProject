/*
 * COMM_CMD.c
 *
 *  Created on: Jun 22, 2019
 *      Author: Hoopoe3n
 */
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Storage/FRAM.h>

#include "COMM_CMD.h"
#include "../../TRXVU.h"
#include "../../COMM/APRS.h"


#define create_task(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask) xTaskCreate( (pvTaskCode) , (pcName) , (usStackDepth) , (pvParameters), (uxPriority), (pxCreatedTask) ); vTaskDelay(10);

void cmd_error(Ack_type* type, ERR_type* err)
{
	*type = ACK_NOTHING;
	*err = ERR_FAIL;
}
void cmd_mute(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	//1. send ACK before mutes satellite
	*type = ACK_MUTE;
	*err = ERR_ACTIVE;
	if (cmd.length != 2)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	//2. mute satellite
	unsigned short param = 	BigEnE_raw_to_uShort(cmd.data);

	int error = set_mute_time(param);
	if (error == 666)
	{
		*err = ERR_PARAMETERS;
	}
	else if (error != 0)
	{
		*err = ERR_FAIL;
	}
}
void cmd_unmute(Ack_type* type, ERR_type* err)
{
	*type = ACK_UNMUTE;
	//1. unmute satellite
	*err = ERR_FRAM_WRITE_FAIL;
	set_mute_time(0);
	unmute_Tx();
	*err = ERR_SUCCESS;
}
void cmd_active_trans(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_TRANSPONDER;
	if (cmd.length != 1)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	//1. checks if the transponder is active
	byte raw[1 + 4];
	// 2.1. convert command id to raw
	BigEnE_uInt_to_raw(cmd.id, &raw[0]);
	// 2.2. copying data to raw
	raw[4] = cmd.data[0];
	// 3. activate transponder
	create_task(Transponder_task, (const signed char * const)"Transponder_Task", 1024, &raw, (unsigned portBASE_TYPE)(configMAX_PRIORITIES - 2), xTransponderHandle);
	//no ACK
}
void cmd_shut_trans(Ack_type* type, ERR_type* err)
{
	*type = ACK_TRANSPONDER;
	//1. shut down the transponder and returning the TRAX to regular transmitting
	sendRequestToStop_transponder();
	*err = ERR_TURNED_OFF;
}
void cmd_change_trans_rssi(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_UPDATE_TRANS_RSSI;
	if (cmd.length != 2)
	{
		*err = ERR_PARAMETERS;
		return;
	}

	unsigned short param = cmd.data[1];
	param += cmd.data[0] << 8;
	if (param > MAX_TRANS_RSSI)
	{
		*err = ERR_PARAMETERS;
		return;
	}

	*err = ERR_SUCCESS;
	change_trans_RSSI(cmd.data);
}
void cmd_aprs_dump(Ack_type* type, ERR_type* err)
{
	*type = ACK_DUMP;

	*err = ERR_NO_DATA;
	//1. send all APRS packets on satellite
	if (send_APRS_Dump())
	{
	//no ACK

	*err=ERR_SUCCESS;
	}
}
void cmd_stop_dump(Ack_type* type, ERR_type* err)
{
	*type = ACK_DUMP;
	//1. stop dump
	sendRequestToStop_dump();
	*err = ERR_TURNED_OFF;
}
void cmd_time_frequency(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_UPDATE_BEACON_TIME_DELAY;
	if (cmd.length != 1)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	//1. check if parameter in range
	if (cmd.data[0] < MIN_TIME_DELAY_BEACON || cmd.data[0] > MAX_TIME_DELAY_BEACON)
	{
		*err = ERR_PARAMETERS;
	}
	//2. update time in FRAM
	else if (!FRAM_writeAndVerify(&cmd.data[0], BEACON_TIME_ADDR, 1))
	{
		*err = ERR_FRAM_WRITE_FAIL;
	}
	else
	{
		*err = ERR_SUCCESS;
	}
}
