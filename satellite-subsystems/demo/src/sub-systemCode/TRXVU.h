/*
 * TRXVU.h
 *
 *  Created on: Oct 20, 2018
 *      Author: elain
 */

#ifndef TRXVU_H_
#define TRXVU_H_

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <satellite-subsystems/IsisTRXVU.h>

#include "Global/Global.h"
#include "COMM/GSC.h"
#define APRS_ON

#define TRXVU_TO_CALSIGN "GS1"
#define TRXVU_FROM_CALSIGN "4x4HSL1"

#define VALUE_TX_BUFFER_FULL 0xff
#define NUM_FILES_IN_DUMP	5

#define NOMINAL_MODE TRUE
#define TRANSPONDER_MODE FALSE

#define MAX_NUTE_TIME 3

#define DEFAULT_TIME_TRANSMITTER (60 * 15)// in seconds

#define GET_BEACON_DELAY_LOW_VOLTAGE(ms) (ms * 3)

#ifndef TESTING
#define DEFULT_BEACON_DELAY 20
#else
#define DEFULT_BEACON_DELAY 20// in seconds
#endif

#define MIN_TIME_DELAY_BEACON	1
#define MAX_TIME_DELAY_BEACON 	40

#define TRANSMMIT_DELAY_9600(length) (length - length)
//(portTickType)((length + 30) * (5 / 6) + 30)
#define TRANSMMIT_DELAY_1200(length) (portTickType)(20 * 100)

#define GROUND_PASSING_TIME	(60*10)//todo: find real values

//todo: find real values
#define DEFULT_COMM_VOL		7250

#define MIN_TRANS_RSSI 	0
#define MAX_TRANS_RSSI 	4095

typedef enum
{
	nothing,
	deleteTask
}queueRequest;

typedef struct isisTXtlm
{
    float tx_reflpwr; 														///< Tx Telemetry reflected power.
    float pa_temp; ///< Tx Telemetry power amplifier temperature.
    float tx_fwrdpwr; ///< Tx Telemetry forward power.
    float tx_current; ///< Tx Telemetry transmitter current.
} isisTXtlm;

typedef struct isisRXtlm {
	float tx_current; ///< Rx Telemetry transmitter current.
	float rx_doppler; ///< Rx Telemetry receiver doppler.
	float rx_current; ///< Rx Telemetry receiver current.
	float bus_volt; ///< Rx Telemetry bus voltage.
	float board_temp; ///< Rx Telemetry board temperature.
	float pa_temp; ///< Rx Telemetry power amplifier temperature.
	float rx_rssi; ///< Rx Telemetry rssi measurement.
} isisRXtlm;

xQueueHandle xDumpQueue;
xQueueHandle xTransponderQueue;//
xTaskHandle xDumpHandle;//task handle for dump task
xTaskHandle xTransponderHandle;//task handle for transponder task

extern time_unix allow_transponder;

/**
 * @brief	check if there's data in the Rx buffer.
 * 			if there's data the function check if the data is command, APRS packet or just Junk
 */
void Rx_logic();

/**
 * @brief	if a command were sent from ground to the satellite this function will be
 * 			in charge of starting the count of time until the pass is over.
 */
void pass_above_Ground();
/**
 * 	@brief		one run of the TRXVU logic
 */
void trxvu_logic();

/**
 * 	@brief 		task function for TRXVU
 */
void TRXVU_task();


/**
 * 	@brief 		task function for dump
 * 	@param[in] 	need to be an unsigned char* (size 9 bytes), data of dump command (packet.data)
 */
void Dump_task(void *arg);


/**
 * 	@brief		task function for transponde mode
 * 	@param[in]	need to be unsigned char* (size 4 bytes), data of command (packet.data)
 */
void Transponder_task(void *arg);


/**
 * 	@brief		build and send beacon packet
 * 	@param[in]	the bitRate to send the beacon in
 */
void buildAndSend_beacon(ISIStrxvuBitrate bitRate);

/**
 * 	@brief	the task in charge of sending a beacon once in defined time
 */
void Beacon_task();


/**
 * 	@brief		send request to Dump_task to delete it self
 * 	@return		0 request send, 1 task does not exists, 2 queue is full
 */
int sendRequestToStop_dump();

/**
 * 	@brief		send request to Transponder_task to delete it self
 * 	@return		0 request send, 1 task does not exists, 2 queue is full
 */
int sendRequestToStop_transponder();

/**
 * @brief		look for request to delete the Dump task, if there's a request
 * 				the function is deleting the Task and saving ACK
 * @param[in]	the ID of the command that started the dump, for saving ACK if there's
 * 				a request to delete the Dump
 */
void lookForRequestToDelete_dump(command_id cmdID);

/**
 * @brief		look for request to delete the transponder task, if there's a request
 * 				the function is deleting the Task and saving ACK
 * @param[in]	the ID of the command that started the dump, for saving ACK if there's
 * 				a request to delete the Dump
 */
void lookForRequestToDelete_transponder(command_id cmdID);

/**
 * @brief		sends data as an AX.25 frame
 * @param[in]	data to send, can't be over
 * @param[in]	length of data to send as an AX.25 frame
 */
int TRX_sendFrame(byte* data, uint8_t length, ISIStrxvuBitrate bitRate);

/**
 * @brief		gets data from Rx buffer
 * @param[in]	length - length of dataBuffer
 * @param[out]	data_out - data from Rx buffer
 * @note		if define SEQUENCE_FLAG the code can handle data across number of frames
 * @return
 */
int TRX_getFrameData(unsigned int *length, byte* data_out);


/**
 * 	@brief		initialize the TRXVU
 */
void init_trxvu(void);

/**
 * @brief	reset the Communication parameters saved on the FRAM to default values
 */
void reset_FRAM_TRXVU();


/**
 * @brief	switching off the mute Tx param
 */
void unmute_Tx();

/**
 * @brief		mute the Tx for a period of time
 * @param[in]	duration in seconds to turn on mute Tx
 */
int set_mute_time(unsigned short time);

/**
 * @brief	check if the duration to mute Tx is over. If the duration is over the function
 * 			exit the mute state
 */
void check_time_off_mute();


/**
 * @brief 					change the Tx to nominal mode or transponder mode
 * @param[in] state			0 for nominal mode, 1 for transponder
 */
void change_TRXVU_state(Boolean state);

/**
 *@brief		change the RSSI to active the Transmit when the transponder mode is active
 *@param[in]	the data to send throw I2C to the TRXVU
 */
void change_trans_RSSI(byte* param);


/**
 *@brief		check the temperature of the local oscillator
 *@return		the temperature of the local oscillator from the TRXVU
 */
temp_t check_Tx_temp();

#endif /* TRXVU_H_ */
