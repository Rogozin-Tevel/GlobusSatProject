/*
 * Global.h
 *
 *  Created on: Oct 20, 2018
 *      Author: Hoopoe3n
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define TESTING

//#define ANTS_ON
#define ANTS_DO_NOT_DEPLOY
#include <hal/boolean.h>

#ifndef BOOLEAN_H_
typedef unsigned int  Boolean;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>

#include <hal/Timing/Time.h>
#include <hal/Storage/FRAM.h>

#include "FRAMadress.h"
#include "sizes.h"

//for tests
//#define NOT_USE_ACK_HK
#define TESTS
#define FILE_SYSTEM_NOT_TESTED
//#define TESTING_BRONFELD
//for tests

#define I2C_BUS_ADDR 0
#define I2C_TRXVU_TC_ADDR 0x61
#define I2C_TRXVU_RC_ADDR 0x60

#define TEMP_VALUE 0

#define SWITCH_ON TRUE
#define SWITCH_OFF FALSE

//states
#define STATE_MUTE 0x01
#define STATE_ALLOW_TRANSPONDER 0x02
//#define STATE_ANTS_DEPLOY 0x04

//delay
#define SYSTEM_DEALY		5
#define TASK_DELAY 100//test...
#define QUEUE_DELAY 100//test...
#define QUEUE_DELETE_DELAY 1000//test...
#define MAX_DELAY 100000//test...
#define ONE_MINUTE	60 * 1000


//tasks buffer
#define BEACON_TASK_BUFFER	2048
#define CAMERA_MANEGER_TASK_BUFFER	4096

// times
#define CONVERT_SECONDS_TO_MS(ms) (ms * 1000)

#define SAVE_TELEMETRY_TIME 60
#define MAIN_ITERATION_TIME 1000
#define DEPLOY_TIME	  2700		// seconds until deployment
//#define DEPLOY_TIME	  10		// seconds until deployment
#define BOOM_DEPLOY_TIME 4     //CHANGE THIS TO 5 BEFORE LAUNCH!!
#define REDEPLOY_TIME	  2700		// seconds until deployment
//#define REDEPLOY_TIME 120

//ants
#define ARM_ANTS 0xAD
#define DISARM_ANTS 0xAC

#define FIRST_ACTIVATION_SIZE 4

#define THREAD_TIMEOUT 60
#define THREAD_LISTENER_TIMEOUT 500

#define MAIN_THREAD 0//0=main 1=mnlp 2=mnlplistener 3=adcs 4=reset
#define ADCS_THREAD 3
#define RESET_THREAD 4

#define STOPTELEM 0 //telemetry position (on/off) 0 for on 1 for off

#define UNIX_EPOCH_TIME_DIFF (30*365*24*3600+7*24*3600)

#define ACTIVE 1
#define UNACTIVE 0

typedef unsigned int time_unix;
typedef unsigned char byte;
typedef unsigned short voltage_t;// in mV
typedef unsigned short current_t;// in mA
typedef float temp_t;// in celsius

typedef enum subSystem_indx
{
	EPS = 0,
	TRXVU = 1,
	Ants = 2,
	ADCS = 4,
	CAMMERA = 5,
	OBC = 6,
	everything = 10
}subSystem_indx;

extern int not_first_activation;

//check error from drivers
void check_int(char *string_output, int error);
void check_portBASE_TYPE(char *string_output, long error);

unsigned int BigEnE_raw_to_uInt(unsigned char raw[4]);
void BigEnE_uInt_to_raw(unsigned int uInt, unsigned char raw[4]);

unsigned short BigEnE_raw_to_uShort(unsigned char raw[2]);

void BigEnE_raw_value(byte *in, int length);

void print_array(unsigned char *arr,int length);
void switch_endian(unsigned char *in, unsigned char *out, int len);
void double_little_endian(unsigned char* d);

int soft_reset_subsystem(subSystem_indx reset_idx);
int hard_reset_subsystem(subSystem_indx reset_idx);

int fram_byte_fix(unsigned int address);
Boolean comapre_string(unsigned char* str0, unsigned char* str1, int numberOfBytes);

Boolean getBitValueByIndex(byte* data, int length, int index);

void reset_FRAM_MAIN();
#endif /* GLOBAL_H_ */
