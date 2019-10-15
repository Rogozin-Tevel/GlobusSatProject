/*
 * sizes.h
 *
 *  Created on: Dec 22, 2018
 *      Author: Hoopoe3n
 */

#ifndef SIZES_H_
#define SIZES_H_


#define IMAGE_WIDTH 2048	///< defines the image width
#define IMAGE_HEIGHT 1088	///< defines the image height

#define CHUNK_WIDTH 14		///< defines the chunk image width
#define CHUNK_HEIGHT 16		///< defines the chunks image height


#define CHUNK_SIZE (CHUNK_HEIGHT*CHUNK_WIDTH)			///< defines number of bytes per chunk
#define NUM_OF_CHUNKS (IMAGE_HEIGHT/CHUNK_HEIGHT)*(IMAGE_WIDTH/CHUNK_WIDTH)	///< defines how many chunks devide an entire image


//variables
#define DOUBLE_SIZE	8
#define FLOAT_SIZE	4
#define INT_SIZE	4
#define CHAR_SIZE	1
#define SHORT_SIZE	2

#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT)	///< image size in bytes
#define MAX_SIZE_OF_PARAM_IMAGE_DUMP	9

#define EPS_VOLTAGES_SIZE 6
#define AUTO_DEPLOYMENT_TIME 10
#define TIME_SIZE 4
#define CMD_ID_SIZE	4
#define ADCS_STAGE_SIZE 10
#define RESTART_FLAG_SIZE 1
#define BEACON_LENGTH 59	//beacon data length
#define ACK_DATA_LENGTH 6

#define SPL_TM_HEADER_SIZE 		8
#define SPL_TC_HEADER_SIZE 		12
#define SPL_TC_HEADER_TIME_PLACE	(SPL_TC_HEADER_SIZE - TIME_SIZE)
#define ACK_RAW_SIZE (SPL_TM_HEADER_SIZE + ACK_DATA_LENGTH)

#define APRS_SIZE_WITH_TIME		18//number of bytes in an APRS packet with time segment
#define APRS_SIZE_WITHOUT_TIME 	14//number of bytes in an APRS packet without time segment

#define SIZE_RXFRAME	200
#define SIZE_TXFRAME	235

#define	MAX_SIZE_TM_PACKET		SIZE_TXFRAME
#define SIZE_OF_COMMAND			SIZE_RXFRAME //max raw size of a command
#define SIZE_OF_DELAYED_COMMAND SIZE_OF_COMMAND//max raw size of a delayed command

#define IMAGE_DATA_FIELD_PACKET_SIZE	CHUNK_SIZE + 2
#define IMAGE_PACKET_SIZE				IMAGE_DATA_FIELD_PACKET_SIZE + SPL_TM_HEADER_SIZE

#define STACK_DUMP_SIZE 2048//when you create the dump task, size of stack, //need to be tasted...
#define DUMP_BUFFER_SIZE  40000

#define EPS_VOLTAGES_SIZE_RAW (EPS_VOLTAGES_SIZE * 2) //!< Size of EPS_VOLTAGES_ADDR

#define STAGE_TABLE_SIZE 9
#endif /* SIZES_H_ */
