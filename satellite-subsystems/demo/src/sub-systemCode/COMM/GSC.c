/*
 * GSC.c
 *
 *  Created on: Oct 20, 2018
 *      Author: Hoopoe3n
 */
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

#include "GSC.h"
#include "splTypes.h"

int decode_TCpacket(byte *data, int length, TC_spl *packet)
{
	if (data == NULL)
		return -1;

	if (length != -1)
	{
		if (length < SPL_TC_HEADER_SIZE || length > SIZE_RXFRAME)
		{
			return 2;
		}
	}
	packet->id = BigEnE_raw_to_uInt(&data[0]);
	packet->type = (byte)data[4];
	packet->subType = (byte)data[5];
	packet->length = (unsigned short)(data[6] << 8);
	packet->length += (unsigned short)data[7];
	if (length != -1)
	{
		if ((int)packet->length + SPL_TC_HEADER_SIZE != length)
		{
			return 1;
		}
		if ((int)packet->length > SIZE_RXFRAME - SPL_TC_HEADER_SIZE)
		{
			return 3;
		}
	}
	packet->time = (time_unix)BigEnE_raw_to_uInt(&data[8]);
	memcpy(packet->data, data + SPL_TC_HEADER_SIZE, (int)packet->length);

	return 0;
}

int encode_TCpacket(byte* data, int *size, TC_spl packet)
{
	if (data == NULL || size == NULL)
		return -1;

	if (packet.length > SIZE_RXFRAME)
		return 1;

	*size = packet.length + SPL_TC_HEADER_SIZE;
	BigEnE_uInt_to_raw(packet.id, &data[0]);
	data[4] = (byte)packet.type;
	data[5] = (byte)packet.subType;
	data[6] = (byte)packet.length >> 8;
	data[7] = (byte)packet.length;
	BigEnE_uInt_to_raw(packet.time, &data[8]);
	memcpy(data + SPL_TC_HEADER_SIZE, packet.data, (int)packet.length);

	return 0;
}

int decode_TMpacket(byte* data, TM_spl *packet)
{
	if (data == NULL)
		return -1;

	packet->type = (byte)data[0];
	packet->subType = (byte)data[1];
	packet->length = (unsigned short)(data[2] << 8);
	packet->length += (unsigned short)data[3];
	if (packet->length > SIZE_TXFRAME - SPL_TM_HEADER_SIZE)
	{
		return 1;
	}
	packet->time = (time_unix)BigEnE_raw_to_uInt(&data[4]);
	memcpy(packet->data, data + SPL_TM_HEADER_SIZE, (int)packet->length);

	return 0;
}

int encode_TMpacket(byte* data, int* size, TM_spl packet)
{
	*size = packet.length + SPL_TM_HEADER_SIZE;

	if (data == NULL || size == NULL)
		return -1;

	if (*size > SIZE_TXFRAME)
		return 1;

	data[0] = (byte)packet.type;
	data[1] = (byte)packet.subType;
	data[2] = (byte)packet.length >> 8;
	data[3] = (byte)packet.length;
	BigEnE_uInt_to_raw(packet.time, &data[4]);
	memcpy(data + SPL_TM_HEADER_SIZE, packet.data, (int)packet.length);

	return 0;
}

int build_raw_ACK(Ack_type type, ERR_type err, command_id ACKcommandId, byte* raw_ACK)
{
	//spl header
	TM_spl spl;
	spl.type = ACK_TYPE;
	spl.subType = 	ACK_ST;
	spl.length = ACK_DATA_LENGTH;
	time_unix time_now;
	Time_getUnixEpoch(&time_now);
	spl.time = time_now;
	//build data field
	build_data_field_ACK(type, err, ACKcommandId, spl.data);
	//encode packet
	int raw_ACK_len = 0;
	return encode_TMpacket(raw_ACK, &raw_ACK_len, spl);
}

int build_data_field_ACK(Ack_type type, ERR_type err, command_id ACKcommandId, byte* data_feild)
{
	if (data_feild == NULL)
		return -1;
	//command id
	BigEnE_uInt_to_raw(ACKcommandId, data_feild);
	//ack type, ack err
	data_feild[4] = (byte)type;
	data_feild[5] = (byte)err;
	return 0;
}
