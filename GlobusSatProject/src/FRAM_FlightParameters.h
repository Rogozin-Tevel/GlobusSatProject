/*
 *
 * @file	FRAM_FlightParameters.h
 * @brief	ordering all flight parameters(saved on the FRAM)  addresses and sizes
 * @note 	sizes are measured in chars = 1 byte. e.g size = 4, meaning 4 bytes(=int)
 */

#ifndef FRAM_FLIGHTPARAMETERS_H_
#define FRAM_FLIGHTPARAMETERS_H_

#include <hal/Storage/FRAM.h>

// <Satellite Management>
#define FIRST_ACTIVATION_FLAG_ADDR		0x42		//<! is this the first activation after launch flag
#define FIRST_ACTIVATION_FLAG_SIZE		4			//<! length in bytes of FIRST_ACTIVATION_FLAG

#define DEPLOYMENT_TIME_ADDR			0X05		//<! time at which the satellites starts deployment
#define DEPLOYMENT_TIME_SIZE			4			//<! size of parameter in bytes

#define MOST_UPDATED_SAT_TIME_ADDR		0x52		//<! this parameters saves the sat time to be read after resets
#define MOST_UPDATED_SAT_TIME_SIZE		4			//<! size of the parameter in bytes

#define NUMBER_OF_RESETS_ADDR			0x60		//<! counts how many restarts did the satellite endure
#define NUMBER_OF_RESETS_SIZE			4			//<! size of the parameter in bytes

#define RESET_CMD_FLAG_ADDR				0x105		//<! the flag is raised whenever a restart is commissioned
#define RESET_CMD_FLAG_SIZE				1			//<! size of the parameter in bytes

#define DEFAULT_EPS_SAVE_TLM_TIME		20			//<! save EPS TLM every 20 seconds
#define EPS_SAVE_TLM_PERIOD_ADDR		0x33		//<! address where the save tlm period will be
#define LAST_EPS_TLM_SAVE_TIME_ADDR		0x20		//<! time of last EPS TLM save inot files

#define DEFAULT_TRXVU_SAVE_TLM_TIME		20			//<! save TRXVU TLM every 20 seconds
#define TRXVU_SAVE_TLM_PERIOD_ADDR		0x33		//<! address where the save tlm period will be
#define LAST_TRXVU_TLM_SAVE_TIME_ADDR	0x20		//<! time of last EPS TLM save inot files

#define DEFAULT_ANT_SAVE_TLM_TIME		20			//<! save antenna TLM every 20 seconds
#define ANT_SAVE_TLM_PERIOD_ADDR		0x33		//<! address where the save tlm period will be
#define LAST_ANT_TLM_SAVE_TIME_ADDR		0x20		//<! time of last EPS TLM save inot files

#define DEFAULT_SOLAR_SAVE_TLM_TIME		20			//<! save solar panel TLM every 20 seconds
#define SOLAR_SAVE_TLM_PERIOD_ADDR		0x33		//<! address where the save tlm period will be
#define LAST_SOLAR_TLM_SAVE_TIME_ADDR	0x20		//<! time of last EPS TLM save inot files

#define DEFAULT_WOD_SAVE_TLM_TIME		20			//<! save WOD TLM every 20 seconds
#define WOD_SAVE_TLM_PERIOD_ADDR		0x33		//<! address where the save tlm period will be
#define LAST_WOD_TLM_SAVE_TIME_ADDR		0x20		//<! time of last EPS TLM save inot files

#define DEFAULT_NO_COMM_WDT_KICK_TIME  (5*24*60*60)	//<! number of seconds in 15 days
#define NO_COMM_WDT_KICK_TIME_ADDR  	0x24		///< number of seconds of no communications before GS WDT kick
#define NO_COMM_WDT_KICK_TIME_SIZE		4

#define LAST_COMM_TIME_ADDR 			0X9485		//<! saves the last unix time at which communication has occured
#define LAST_COMM_TIME_SIZE				4			//<! size of last communication time in bytes

#define LAST_WAKEUP_TIME_ADDR			0X40		//<! saves the first time after satellites wakeup from reset
#define LAST_WAKEUP_TIME_SIZE			4			//<! size of the parameter in bytes
// </Satellite Management>

// <TRXVU>
#define DELAYED_CMD_BUFFER_ADDR			0x8000		//<! starting address of the delayed command buffer
#define DELAYED_CMD_BUFFER_LENGTH		50			//<! number of commands the buffer is capable of saving(self defined, can be changed)

#define DEFALUT_BEACON_BITRATE_CYCLE	3
#define BEACON_BITRATE_CYCLE_ADDR		0X4587		//<! saves the number of beacons with period of 3
#define BEACON_BITRATE_CYCLE_SIZE		1			//<! length in bytes of BEACON_PERIOD

#define DEFAULT_BEACON_INTERVAL_TIME 	20			//<! how many seconds between two beacons [sec]
#define MAX_BEACON_INTERVAL				60			// beacon every 1 minute
#define MIN_BEACON_INTERVAL				10			// beacon every 10 seconds
#define BEACON_INTERVAL_TIME_ADDR 		0x4590		//<! address of value of the delay between 2 beacons
#define BEACON_INTERVAL_TIME_SIZE 		4			//<! size of parameter in bytes


#define DELAYED_CMD_FRAME_COUNT_ADDR	0X200		//<! Address where amount of frames in delayed buffer is held
#define DELAYED_CMD_FRAME_COUNT_SIZE	1			//<! number of bytes describing the frame count of delayed buffer

#define TRANS_ABORT_FLAG_ADDR			0x500		//<! transmission abort request flag
#define TRANS_ABORT_FLAG_SIZE			1			//<! size of mute flag in bytes
// </TRXVU>

// <EPS>

#define EPS_THRESH_VOLTAGES_ADDR		0x666		//<! starting address for eps threshold voltages array
#define EPS_THRESH_VOLTAGES_SIZE (NUMBER_OF_THRESHOLD_VOLTAGES * sizeof(voltage_t)) //<! number of bytes in eps threshold voltages array

#define EPS_ALPHA_FILTER_VALUE_ADDR 0x550			//<! filtering value in the LPF formula
#define EPS_ALPHA_FILTER_VALUE_SIZE sizeof(float)	//<! size of double (alpha)
// </EPS>


#endif /* FRAM_FLIGHTPARAMETERS_H_ */
