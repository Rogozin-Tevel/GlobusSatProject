/*
 * GSC.h
 *
 *  Created on: Oct 20, 2018
 *      Author: Hoopoe3n
 */

#ifndef GSC_H_
#define GSC_H_

#include "../Global/Global.h"
#include "splTypes.h"

#define MAX_NAMBER_OF_APRS_PACKETS 20//the max number of APRS packets in the FRAM list
#define NO_AVAILABLE_SLOTS -1

typedef enum ERR_type
{
	ERR_SUCCESS,
	ERR_FAIL,
	ERR_ACTIVE,
	ERR_STOP_TASK,
	ERR_TURNED_OFF,
	ERR_PARAMETERS,
	ERR_FRAM_WRITE_FAIL,
	ERR_FRAM_READ_FAIL,
	ERR_NO_DATA,
	ERR_WRITE,
	ERR_NOT_INITIALIZED,
	ERR_TASK_EXISTS,
	ERR_TETST,
	ERR_DONE,
	ERR_SYSTEM_OFF,
	ERR_ERROR,
	ERR_BOOT_LOADER_STUCK,
	IMAGE_ERR_START = 30,
	IMAGE_ERR_END = 75
}ERR_type;

//ACK enum
typedef enum ACK
{
	ACK_RECEIVE_COMM = 0,					//ACK when receive any packet
	ACK_CAMERA = 1,
	ACK_ADCS_SET_BOOT_LODER = 30,
	ACK_ADCS_RUN_BOOT_LOADER = 31,
	ACK_ADCS_UPFATE_TLE_PARAMETER = 32,
	ACK_ADCS_SAVE_TLE = 33,
	ACK_ADCS_CONFIG = 34,
	ACK_ADCS_SAVE_CONFIG = 35,
	ACK_ADCS_STAGE_TABLE = 36,
	ACK_ADCS_SET_TIME = 37,
	ACK_ADCS_BOOM_DEPLOY = 38,

	ACK_THE_MIGHTY_DUMMY_FUNC = 126,			//be aware of the mighty dummy func
	ACK_RESET = 127,
	ACK_SW_RESET = 128,
	ACK_HARD_RESET = 129,
	ACK_SOFT_RESTART = 130,					//ACK before going to reset
	ACK_MEMORY = 131,							//ACK when memory delete is completed success
	ACK_TLC_RECIEVED = 132,					//when after receiving TLC
	ACK_DELETE_IMAGE_FROM_MEMORY_CAM = 133,	//when deleting image form camera SD
	ACK_MOVE_IMAGE_CAM_OBC = 134, 				//after moving image to OBC SD from camera SD
	ACK_START_TRANSPONDER = 135,				//ACK before activating transponder
	ACK_UPDATE_TIME = 136,					//after updating time
	ACK_UPDATE_BEACON_BIT_RATE = 137,
	ACK_UPDATE_BEACON_TIME_DELAY = 138,
	ACK_UPDATE_EPS_VOLTAGES = 139,
	ACK_UPDATE_EPS_HEATER_VALUES = 140,
	ACK_MUTE = 141,
	ACK_UNMUTE = 142,
	ACK_ALLOW_TRANSPONDER = 143,
	ACK_TRANSPONDER = 144,
	ACK_DUMP = 145,
	ACK_IMAGE_DUMP = 146,
	ACK_GENERIC_I2C_CMD = 147,
	ACK_ARM_DISARM = 148,				//after changing arm state of the ants
	ACK_REDEPLOY = 149,
	ACK_UPDATE_COMM_VOLTAGES = 157,
	ACK_RESET_DELAYED_CMD = 158,
	ACK_RESET_APRS_LIST = 159,
	ACK_FRAM_RESET = 160,
	ACK_UPDATE_TRANS_RSSI = 161,
	ACK_EPS_SHUT_SYSTEM = 162,
	ACK_RESET_FILE = 163,
	ACK_NOTHING = 255
}Ack_type;

typedef unsigned int command_id;

typedef struct inklajn_spl_TM
{
	uint8_t type;//service type
	uint8_t subType;//service sub type
	unsigned short length;//Length of data array
	time_unix time;//Unix time
	byte data[SIZE_TXFRAME - SPL_TM_HEADER_SIZE];//the data in the packet
}TM_spl;

typedef struct inklajn_spl_TC
{
	command_id id;
	uint8_t type;//service type
	uint8_t subType;//service sub type
	unsigned short length;//Length of data array
	time_unix time;//Unix time
	byte data[SIZE_OF_COMMAND - SPL_TC_HEADER_SIZE];//the data in the packet
}TC_spl;

/**
 *	@brief			decode raw data to a TM spl packet
 *	@param[in]		data to decode
 *	@param[in]		length of data to decode
 *	@param[in][out]	decoded TM spl packet
 *	@return			0 no problems in decoding,
 *					1 ,2 ,3 a problem with length
 *					-1 a NULL pointer
 */
int decode_TMpacket(byte* data, TM_spl* packet);//this function use calloc for the data

/**
 *	@brief			encode TM spl packet to raw data
 *	@param[out] 	data encoded data
 *	@param[out]		size length of *data
 *	@param[in]		packet TM spl packet to encode
 *	@return			0 no problems in decoding,
 *					1 a problem with length
 *					-1 a NULL pointer
 */
int encode_TMpacket(byte* data, int* size, TM_spl packet);//this function use calloc for the data

/**
 *	@brief			decode raw data to a TC spl packet
 *	@param[in]		data to decode
 *	@param[in]		length of data to decode
 *	@note			if length is -1 that means that the command is delayed command
 *	@param[in][out]	decoded TC spl packet
 *	@return			0 no problems in decoding,
 *					1 a problem with length
 *					-1 a NULL pointer
 */
int decode_TCpacket(byte* data, int length, TC_spl* packet);//this function use calloc for the data

/**
 *	@brief			encode TC spl packet to raw data
 *	@param[out] 	data encoded data
 *	@param[out]		size length of *data
 *	@param[in]		packet TC spl packet to encode
 *	@return			0 no problems in decoding,
 *					1 a problem with length
 *					-1 a NULL pointer
 */
int encode_TCpacket(byte* data, int* size, TC_spl packet);//this function use calloc for the data

/**
 * @brief 		build Ack inside spl.
 * @param[in] 	type Ack type according to Ack_type (typedef enum).
 * @param[in]	err Errors to be send.
 * return		ACK packet as inklajn_spl
 */
int build_raw_ACK(Ack_type type, ERR_type err, command_id ACKcommandId, byte* raw_ACK);

/**
 * @brief 		build raw ack
 * @param[in] 	type Ack type according to Ack_type (typedef enum).
 * @param[in]	err Errors to be send.
 * @param[in]	the array to fill the the raw ack
 * return		0
 * 				-1 a NULL pointer
 */
int build_data_field_ACK(Ack_type type, ERR_type err, command_id ACKcommandId, byte* data_feild);
#endif /* GSC_H_ */
