/*
 * APRS.c
 *
 *  Created on: Jun 23, 2019
 *      Author: Hoopoe3n
 */
#include <hal/Storage/FRAM.h>

#include <hal/Timing/Time.h>

#include "APRS.h"
#include "GSC.h"
#include "../TRXVU.h"

static byte APRS_list[APRS_SIZE_WITH_TIME * MAX_NAMBER_OF_APRS_PACKETS];

void reset_APRS_list(Boolean firstActivation)
{
	int i_error = 0;
	uint8_t numberOfPackets = 0;	//get number of packets in the FRAM

	memset(APRS_list, 0, APRS_SIZE_WITH_TIME * MAX_NAMBER_OF_APRS_PACKETS);	//sets all slots in the array to zero for later write to the FRAM

	// write 0 to all the FRAM list
	i_error = FRAM_write(APRS_list, APRS_PACKETS_ADDR, (APRS_SIZE_WITH_TIME * MAX_NAMBER_OF_APRS_PACKETS));//reset the memory of the delay command list in the FRAM
	check_int("reset_APRS_list, FRAM_write", i_error);

	// write 0 in number of APRS
	i_error = FRAM_write(&numberOfPackets, NUMBER_PACKET_APRS_ADDR, 1);//reset the number of commands in the FRAM to 0
	check_int("reset_APRS_list, FRAM_write", i_error);

	//  if its not init, update the number of APRS commands
	if (!firstActivation)
	{
		set_numOfAPRS(numberOfPackets);
	}
}

int send_APRS_Dump()
{
	int i_error = 0;
	byte numberOfAPRS = 0;	// number of packets in APRS_packets

	// 1. Get the list of APRS packets and number of packets from the FRAM
	i_error = FRAM_read(&numberOfAPRS, NUMBER_PACKET_APRS_ADDR, sizeof(numberOfAPRS));
	check_int("send_APRS_Dump, FRAM_read", i_error);
	i_error =FRAM_read(APRS_list, APRS_PACKETS_ADDR, (numberOfAPRS * APRS_SIZE_WITH_TIME));
	check_int("send_APRS_Dump, FRAM_read", i_error);

	if (numberOfAPRS == 0)
	{
		return 1;
	}

	time_unix time_now;
	byte rawData[APRS_SIZE_WITH_TIME + 8];
	int rawDataLength = 0;

	TM_spl packet;
	packet.type = APRS;
	packet.subType = APRS_PACKET_FRAM;
	packet.length = APRS_SIZE_WITH_TIME;

	// 2. Going throw every packet on the list
	int i, j;
	for (i = 0; i < numberOfAPRS; i++)
	{
		// 3. Insert data to packet
		memcpy(packet.data, APRS_list, APRS_SIZE_WITH_TIME);

		i_error = Time_getUnixEpoch(&time_now);	//get time
		check_int("send_APRS_Dump, Time_getUnixEpoch", i_error);
		packet.time = time_now;

		encode_TMpacket(rawData, &rawDataLength, packet);
		// 4. Sends packet twice
		for (j = 0; j < 2; j++)
		{
			TRX_sendFrame(rawData, (unsigned char)rawDataLength, trxvu_bitrate_9600);
		}
	}

	// 5. Reseting the APRS list in the FRAM
	reset_APRS_list(FALSE);

	return 0;
}

int check_APRS(unsigned char* data)
{
	//1. checks if the packet is APRS packet
	char PrefixAPRS[] = { '!' };
	if (!memcmp(PrefixAPRS, data, 1))
	{
		//2. Add Time stamp
		time_unix time_now;
		Time_getUnixEpoch(&time_now);
		BigEnE_uInt_to_raw(time_now, &data[APRS_SIZE_WITHOUT_TIME]);

		//3. save APRS Packet in the FRAM list
		uint8_t number_of_APRS_save = 0;
		FRAM_read(&number_of_APRS_save, NUMBER_PACKET_APRS_ADDR, sizeof(number_of_APRS_save));

		if (number_of_APRS_save == 0)	//in case there's no list in the FRAM
		{
			FRAM_write(data, APRS_PACKETS_ADDR, APRS_SIZE_WITH_TIME);//write the list back to the FRAM
			number_of_APRS_save++;
			FRAM_write(&number_of_APRS_save, NUMBER_PACKET_APRS_ADDR, sizeof(number_of_APRS_save));//write the number of APRS packets in the FRAM back to the FRAM

			return 1;	//returns that the packet was an APRS packet
		}

		FRAM_read(APRS_list, APRS_PACKETS_ADDR, (MAX_NAMBER_OF_APRS_PACKETS * APRS_SIZE_WITH_TIME));//read exiting list in the FRAM
		int count = 0;
		while (APRS_list[count] == '!')//find if empty place for the new APRS packet
		{
			count += APRS_SIZE_WITH_TIME;
		}

		//saves the APRS packet in the exiting list
		for (int i = 0; i < APRS_SIZE_WITH_TIME; i++)
		{
			APRS_list[i + count] = data[i];
		}

		FRAM_write(APRS_list, APRS_PACKETS_ADDR, APRS_SIZE_WITH_TIME);//write the list back to the FRAM
		number_of_APRS_save++;
		FRAM_write(&number_of_APRS_save, NUMBER_PACKET_APRS_ADDR, sizeof(number_of_APRS_save));//write the number of APRS packets in the FRAM back to the FRAM
		set_numOfAPRS(number_of_APRS_save);

		return 1;	//returns that the packet was an APRS packet
	}

	return 0;
}

void get_APRS_list()
{
	int error;

	memset(APRS_list, 0, APRS_SIZE_WITH_TIME * MAX_NAMBER_OF_APRS_PACKETS);	//sets all slots in the array to zero for later write to the FRAM
	error = FRAM_read(APRS_list, APRS_PACKETS_ADDR, APRS_SIZE_WITH_TIME * MAX_NAMBER_OF_APRS_PACKETS);
	check_int("get_APRS_list, FRAM_read(APRS_PACKETS_ADDR)", error);

	uint8_t num_of_APRS_save;
	error = FRAM_read(&num_of_APRS_save, NUMBER_PACKET_APRS_ADDR, 1);
	check_int("get_APRS_list, FRAM_read(NUMBER_PACKET_APRS_ADDR)", error);

	set_numOfAPRS(num_of_APRS_save);
}
