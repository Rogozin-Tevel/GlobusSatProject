/*
 ============================================================================
 Name        : ADCS.c
 Author      : Michael
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
//! define True as 1 true in c
//#define TRUE 1
//! define False as 0 false in c
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <stdio.h>
#include <stdlib.h>

#include <hcc/api_fat.h>

#include "../ADCS.h"
#include "../Global/GlobalParam.h"

// include for TRXVU test
#include <satellite-subsystems/IsisTRXVU.h>

//! a function that is called once and it starts the adcs loop and an initialization of the cubeadcs
void init_adcs(Boolean activation)
{
	unsigned char I2C = ADCS_I2C_ADRR;
	// connects to the cubeADCS thorough I2C
	int err = cspaceADCS_initialize(&I2C,1);
	if(err != 0)
	{
		printf("%d",err);
	}
	vTaskDelay(2000);
	// well all files need to be created, don't they? this function creates them
	// Initializes the stage table. you now empty and alone like me :|

	//initialize the stage table
	stageTable ST = get_ST();
	byte raw_stageTable[STAGE_TABLE_SIZE];
	if (activation)
	{
		createTable(ST);
		getTableTo(get_ST(), raw_stageTable);
		FRAM_write(raw_stageTable, STAGE_TABLE_ADDR, STAGE_TABLE_SIZE);
	}
	else
	{
		FRAM_read(raw_stageTable, STAGE_TABLE_ADDR, STAGE_TABLE_SIZE);
		translateCommandFULL(raw_stageTable, ST);
	}
	// saves the stage table to the fram
	cspace_adcs_runmode_t Run = runmode_enabled;
	err = cspaceADCS_setRunMode(ADCS_ID,Run);
	if(err != 0)
	{
		printf("/n %d run",err);
	}
}

