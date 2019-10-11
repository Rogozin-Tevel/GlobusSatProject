/*
 * GlobalParam.h
 *
 *  Created on: Mar 28, 2019
 *      Author: elain
 */

#ifndef GLOBALPARAM_H_
#define GLOBALPARAM_H_

#include "sizes.h"
#include "Global.h"
#include "../ADCS/Stage_Table.h"

typedef enum
{
	mute_param,
	anttena_deploy_param,
	transponder_active_param,
	dump_param,
	//state of the sub-system (on/off)
	cam_param,
	cam_operational_param,
	Tx_param,
	ADCS_param,
}systems_state_parameters;

typedef union __attribute__ ((__packed__))
{
	byte raw;
	struct __attribute__ ((__packed__))
	{
		byte mute : 1,
		anttena_deploy : 1,
		transponder_active : 1,
		dump : 1,
		//state of the sub-system (on/off)
		cam_operational : 1,
		Tx : 1,
		ADCS : 1,
		cammera : 1;
	}fields;
}systems_state;

typedef struct __attribute__ ((__packed__))
{
	voltage_t Vbatt;//current voltage of the batteries
	voltage_t pre_vbatt[3];
	current_t curBat;//
	current_t cur3V3;
	current_t cur5V;

	unsigned short tempComm_LO;//current temperature of the trxvu
	unsigned short tempComm_PA;//current temperature of the trxvu
	short tempEPS[4];//current temperature of the EPS controller
	short tempBatt[2];//current temperature of the batteries

	unsigned short RxDoppler;
	unsigned short RxRSSI;
	unsigned short TxRefl;
	unsigned short TxForw;

	stageTable ST;
	short Attitude[3];

	uint8_t numOfPics;//number of pictures saved in the file system of the satellite
	uint8_t numOfAPRS;//number of APRS packet saved in the FRAM
	uint8_t numOfDelayedCommand;//number of delayed command saved on the FRAM
	unsigned int numOfResets;
	time_unix lastReset;

	systems_state state;

	Boolean ground_conn;
} global_param;

int init_GP();

Boolean get_system_state(systems_state_parameters param);
void set_system_state(systems_state_parameters param, Boolean set_state);

// get the whole global structure.
void get_current_global_param(global_param* param_out);
//	CGP->current Vbatt
voltage_t get_Vbatt();
void set_Vbatt(voltage_t param);
//	CGP->Vbatt privouse
void set_Vbatt_previous(voltage_t vbatt_prev[3]);
void get_Vbatt_previous(voltage_t *vbatt_prev);
// CGP ->current system
void set_curBat(current_t param);
current_t get_curBat();
// CGP-> cur3V3
current_t get_cur3V3();
void set_cur3V3(current_t param);
// CGP-> cur5V
current_t get_cur5V();
void set_cur5V(current_t param);
// CGP-> tempComm_LO
temp_t get_tempComm_LO();
void set_tempComm_LO(unsigned short param);
// CGP-> tempComm_PA
temp_t get_tempComm_PA();
void set_tempComm_PA(unsigned short param);
// CGP-> tempEPS
temp_t get_tempEPS(int index);
void set_tempEPS(int index, short param);
// CGP-> tempBatt
temp_t get_tempBatt(int index);
void set_tempBatt(int index, short param);
// CGP-> RxDoppler
unsigned short get_RxDoppler();
void set_RxDoppler(unsigned short param);
// CGP-> RxRSSI
unsigned short get_RxRSSI();
void set_RxDoppler(unsigned short param);
// CGP-> RxRSSI
unsigned short get_RxRSSI();
void set_RxRSSI(unsigned short param);
// CGP-> TxRefl
unsigned short get_TxRefl();
void set_TxRefl(unsigned short param);
// CGP-> TxFrow
unsigned short get_TxForw();
void set_TxForw(unsigned short param);
// CGP-> ST
stageTable get_ST();
// CGP-> Attitude
short get_Attitude(int index);
void set_Attitude(int index, short param);
// CGP-> numOfPics
uint8_t get_numOfPics();
void set_numOfPics(uint8_t param);
// CGP-> numOfAPRS
uint8_t get_numOfAPRS();
void set_numOfAPRS(uint8_t param);
// CGP-> numOfDelayedCommand
uint8_t get_numOfDelayedCommand();
void set_numOfDelayedCommand(uint8_t param);
// CGP ->number of restarts
void set_numOfResets(unsigned int num);
unsigned int get_numOfResets();
// CGP ->lastReset
void set_lastReset(time_unix param);
time_unix get_lastReset();
//CGP ->connection to ground state
void set_ground_conn(Boolean param);
Boolean get_ground_conn();
#endif /* GLOBALPARAM_H_ */
