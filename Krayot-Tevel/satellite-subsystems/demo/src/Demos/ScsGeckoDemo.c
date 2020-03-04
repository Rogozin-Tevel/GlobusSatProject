/*
 * ScsGeckoDemo.c
 *
 *  Created on: 29 jan. 2018
 *      Author: pbot
 */

#include <satellite-subsystems/SCS_Gecko/gecko_driver.h>
#include <satellite-subsystems/SCS_Gecko/gecko_use_cases.h>

#include <hal/Timing/WatchDogTimer.h>
#include <hal/Drivers/SPI.h>
#include <hal/Utility/util.h>
#include <hal/boolean.h>
#include <hal/errors.h>

#include <hcc/api_fat.h>
#include <hcc/api_hcc_mem.h>
#include <hcc/api_fat_test.h>
#include <hcc/api_mdriver_atmel_mcipdc.h>
#include <hal/Drivers/I2C.h>
#include <stdint.h>
#define _SD_CARD 0
#define SOFT_RESET 0xF8
#define TURN_OFF_FPGA_POWER 0x55
#define PREFORM_READBACK 0x1C
#define ERASE_FPGA 0x32
#define CLEAR_WDT_AND_ALLOW_READ 0x3F
#define READ_ORMADD 0xE3
#define SOREQ_I2C_ADDRESS 0x55
#define READ_PIC32_RESETS 0x66
#define PIC32_LATCH_UP_COUNTER 0xE3
#define READ_SMP_SAMLE_DATA 0x44
#define MEASUER_SMPA 0xCC
#define OPEN_SMPA 0xAA
#define STOP_SMPA_OPENING 0xBB
#define READ_RADFET_VOLTAGES 0x33
#define NUMBER_OF_TESTED_FPGA_2 0x65
#define NUMBER_OF_TESTED_FPGA_3 0x76
#define NUMBER_OF_TESTED_FPGA_4 0x88
#define READ_TOTAL_TESTES_FPGAA 0x89
#define TURN_OFF_FPGA 0x55
#define TURN_ON_FPGA_POWER 0x07
#define PREFORM_READBACK 0x1C

int sendCommand(unsigned char command,unsigned char result[],int size)
{
	unsigned char wtd_and_read[] = {CLEAR_WDT_AND_ALLOW_READ};
	I2C_write(SOREQ_I2C_ADDRESS,&command,1);
	vTaskDelay(100);
	//while(result[3]!=0)
	{
		I2C_write(0x55,wtd_and_read,1);
		vTaskDelay(100);
		I2C_read(SOREQ_I2C_ADDRESS,result,size);
		printf("sendCommand statuc is : %X\n",result[3]);

	}
	return TRUE;
}
static unsigned char res[2000];
void print_res(char* arr,int size)
{
	printf("res:\n");
	int i;
	for(i =0; i<size;i++)
	{
		printf("data[%d] = %08X\n",i,((int*)arr)[i]);
	}
	printf("\n end \n");
}
static Boolean dolevTest()
{

	res[3]=0;
	char command = OPEN_SMPA;
	unsigned char wtd_and_read[] = {CLEAR_WDT_AND_ALLOW_READ};
	I2C_write(SOREQ_I2C_ADDRESS,&command,1);
	vTaskDelay(100);
	int i = 0;
	while(res[3]!=0)
	{
		I2C_write(0x55,wtd_and_read,1);
		vTaskDelay(100);
		I2C_read(SOREQ_I2C_ADDRESS,res,4);
		printf("sendCommand statuc is : %X\n",res[3]);
		if((i++)==10)
		{
			command = STOP_SMPA_OPENING;
			I2C_write(SOREQ_I2C_ADDRESS,&command,1);
		}
	}
	return TRUE;
}

Boolean selectAndExecuteGeckoDemoTest( void )
{
	unsigned int selection = 0;
	Boolean offerMoreTests = TRUE;

	printf("\n\r Select a test to perform: \n\r");
	printf("\t 0) Return to main menu \n\r");
	printf("\t 1) SOFT_RESET \n\r");
	printf("\t 2) Clear watchdog timer , allow read of BUSY flag and data \n\r");
	printf("\t 3) Read the total PIC32 resets from the PIC itself (due to power cycling, watchdog reset and soft reset) \n\r");
	printf("\t 4) PIC32 Latch Up counter: read counter from FPGA spartan (8 bits) \n\r");
	printf("\t 5) Read SMP sampled data after PIC32 power off due to latch-up \n\r");
	printf("\t 6) Measure SMPA resistance and then turn off the voltage \n\r");
	printf("\t 7) Open SMPA \n\r");
	printf("\t 8) Stop SMPA opening \n\r");
	printf("\t 9) Read radfet voltages \n\r");
	printf("\t 10) Define number of tested FPGAs = 2 \n\r");
	printf("\t 11) Define number of tested FPGAs = 3 \n\r");
	printf("\t 12) Define number of tested FPGAs = 4 \n\r");
	printf("\t 13) Read total tested FPGAs (debug purpose) \n\r");
	printf("\t 14) Turn off FPGA power \n\r");
	printf("\t 15) Turn on FPGA power and reconfig FPGAs \n\r");
	printf("\t 16) Open SMPA for 1 SEC \n\r");
	printf("\t 17) Perform readback \n\r");


	while(UTIL_DbguGetIntegerMinMax(&selection, 0, 17) == 0);

	switch (selection)
	{
	case 0:
		offerMoreTests = FALSE;
		break;
	case 1:
		offerMoreTests = sendCommand(SOFT_RESET,res,4);
		break;
	case 2:
			printf("how much to read? \n");
			while(UTIL_DbguGetIntegerMinMax(&selection, 1, 5000) == 0);
			offerMoreTests = sendCommand(CLEAR_WDT_AND_ALLOW_READ,res,selection);
			print_res(res,selection);
			break;
	case 3:
			offerMoreTests = sendCommand(READ_PIC32_RESETS,res,8);
			printf("num of resets: %d\n",((int*)res)[2]);
			break;
	case 4:
			offerMoreTests = sendCommand(PIC32_LATCH_UP_COUNTER,res,8);
			printf("num of LATCH UP: %d\n",((int*)res)[2]);
			break;
	case 5:
		printf("how much to read? \n");
			while(UTIL_DbguGetIntegerMinMax(&selection, 1, 8000) == 0);
			offerMoreTests = sendCommand(READ_SMP_SAMLE_DATA,res,selection);
			print_res(res,selection);
			break;
	case 6:
			offerMoreTests = sendCommand(MEASUER_SMPA,res,4);
			printf("ADC conversion SMPA: %d",((int*)res)[2]);
			break;
	case 7:
			offerMoreTests = sendCommand(OPEN_SMPA,res,4);
			break;
	case 8:
			offerMoreTests = sendCommand(STOP_SMPA_OPENING,res,4);
			break;
	case 9:
			offerMoreTests = sendCommand(READ_RADFET_VOLTAGES,res,4);
			break;
	case 10:
			offerMoreTests = sendCommand(NUMBER_OF_TESTED_FPGA_2,res,4);
			break;
	case 11:
			offerMoreTests = sendCommand(NUMBER_OF_TESTED_FPGA_3,res,4);
			break;
	case 12:
			offerMoreTests = sendCommand(NUMBER_OF_TESTED_FPGA_4,res,4);
			break;
	case 13:
			offerMoreTests = sendCommand(READ_TOTAL_TESTES_FPGAA,res,4);
			break;
	case 14:
			offerMoreTests = sendCommand(TURN_OFF_FPGA,res,4);
			break;
	case 15:
			offerMoreTests = sendCommand(TURN_ON_FPGA_POWER,res,4);
			break;
	case 16:
				offerMoreTests = dolevTest();
			break;
	case 17:
		printf("how much to read? \n");
		while(UTIL_DbguGetIntegerMinMax(&selection, 1, 8000) == 0);
		offerMoreTests = sendCommand(PREFORM_READBACK,res,selection);
		print_res(res,selection);
	default:
		break;
	}

	return offerMoreTests;
}

int _InitFileSystem( void )
{
	int ret;

	hcc_mem_init(); /* Initialize the memory to be used by filesystem */

	ret = fs_init(); /* Initialize the filesystem */
	if(ret != F_NO_ERROR )
	{
		TRACE_ERROR("fs_init pb: %d\n\r", ret);
		return -1;
	}

	ret = f_enterFS(); /* Register this task with filesystem */
	if(ret != F_NO_ERROR )
	{
		TRACE_ERROR("f_enterFS pb: %d\n\r", ret);
		return -1;
	}

	ret = f_initvolume( 0, atmel_mcipdc_initfunc, _SD_CARD ); /* Initialize volID as safe */

	if( F_ERR_NOTFORMATTED == ret )
	{
		TRACE_ERROR("Filesystem not formated!\n\r");
		return -1;
	}
	else if( F_NO_ERROR != ret)
	{
		TRACE_ERROR("f_initvolume pb: %d\n\r", ret);
		return -1;
	}

	return 0;
}

void _DeinitFileSystem( void )
{
	f_delvolume( _SD_CARD ); /* delete the volID */
	f_releaseFS(); /* release this task from the filesystem */

	fs_delete(); /* delete the filesystem */

	hcc_mem_delete(); /* free the memory used by the filesystem */
}


void ScsGeckoDemoLoop(void)
{
	Boolean offerMoreTests = FALSE;

	while (1)
	{
		offerMoreTests = selectAndExecuteGeckoDemoTest();

		if (offerMoreTests == FALSE)
		{
			break;
		}
	}
}

Boolean ScsGeckoDemoMain(void)
{
	if(TRUE)
	{
		ScsGeckoDemoLoop();

		//_DeinitFileSystem();

		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

Boolean GeckoTest( void )
{
	ScsGeckoDemoMain();
	return TRUE;
}


