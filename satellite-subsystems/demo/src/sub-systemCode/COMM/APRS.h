/*
 * APRS.h
 *
 *  Created on: Jun 23, 2019
 *      Author: Hoopoe3n
 */

#ifndef APRS_H_
#define APRS_H_

#include "../Global/Global.h"
#include "../Global/GlobalParam.h"


/**
 * 	@brief 		reset the APRS packet list in the FRAM, deleting all packets
 */
void reset_APRS_list(Boolean firstActivation);

/**
 * 	@brief		send all APRS packets from the FRAM list and reseting it
 */
int send_APRS_Dump();

/**
 *  @brief		checks if the data we got from ground is an APRS packet or an ordinary data
 *  @param[in]	bytes array, the data we got from the ground
 *  @param[in]	the length of unsigned char* data
 *  @return		0 if ordinary data, 1 if APRS data
 */
int check_APRS(byte* data);

/*
 * @brief reads the APRS from the FRAM to a buffer
 */
void get_APRS_list();

#endif /* APRS_H_ */
