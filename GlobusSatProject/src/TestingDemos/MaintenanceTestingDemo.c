#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "MaintenanceTestingDemo.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include <stdio.h>
#include <hal/Utility/util.h>
#include <hal/Timing/Time.h>
#include <hcc/api_fat.h>

Boolean TestIsFS_Corrupted()
{
	FN_SPACE space;
	int drivenum = f_getdrive();

	f_getfreespace(drivenum, &space);

	printf("Number of 'bad' bits: %lu\n",space.bad);
	Boolean is_corrupted = IsFS_Corrupted();
	char bool[10] = {0};

	if(is_corrupted)
		sprintf(bool,"TRUE");
	else
		sprintf(bool,"FALSE");

	printf("is System corrupted: %s",bool);
	return TRUE;
}

Boolean TestWakeupFromResetCMD()
{
	printf("Testing WakupFromReset:\n");
	WakeupFromResetCMD();
	printf("Finished\n");
	return TRUE;
}

Boolean TestResetGroundCommWDT()
{
	time_unix current_time = 0;

	FRAM_read((unsigned char*) &current_time, LAST_COMM_TIME_ADDR,LAST_COMM_TIME_SIZE);
	printf("Saved time on the FRAM: %lu\n",current_time);

	ResetGroundCommWDT();

	Time_getUnixEpoch(&current_time);
	printf("Current Time according to 'Time_getUnixEpoch': %lu\n",current_time);

	FRAM_read((unsigned char*) &current_time, LAST_COMM_TIME_ADDR,LAST_COMM_TIME_SIZE);
	printf("Saved time on FRAM after reset: %lu\n",current_time);


	return TRUE;
}

Boolean TestIsGroundCommunicationWDTKick()
{
	Boolean wdt_kick = IsGroundCommunicationWDTKick();
	char bool[10] = {0};

	if(wdt_kick)
		sprintf(bool,"TRUE");
	else
		sprintf(bool,"FALSE");

	printf("Did GS WDT kick: %s\n",bool);
	return TRUE;
}

Boolean TestMaintenanceMainLoop()
{
	int minutes = 2;
		printf("\nPlease insert number of minutes to test(0 to 10)\n");
		while (UTIL_DbguGetIntegerMinMax((unsigned int*)&minutes, 0, 10) == 0);

		printf("Starting test for a period of %d minutes\n\n",minutes);

		printf("starting test...\n");

		portTickType curr_time = xTaskGetTickCount();
		portTickType end_time = MINUTES_TO_TICKS(minutes) + curr_time;

		printf("Start tick count: %d\n",(int)curr_time);
		printf("End tick count: %d\n",(int)end_time);

		while( curr_time < end_time)
		{
			curr_time = xTaskGetTickCount();
			printf("seconds till finish: %d\n",(int)(end_time - curr_time)/1000);

			Maintenance();
			vTaskDelay(1000);
		}
		return TRUE;

	return TRUE;
}

Boolean selectAndExecuteMaintenanceDemoTest()
{
	unsigned int selection = 0;
	Boolean offerMoreTests = TRUE;

	printf("\n\r Select a test to perform: \n\r");
	printf("\t 0) Return to main menu \n\r");
	printf("\t 1) Test IsFS_Corrupted \n\r");
	printf("\t 2) Test WakeupFromResetCMD\n\r");
	printf("\t 3) Test ResetGroundCommWDT\n\r");
	printf("\t 4) Test IsGroundCommunicationWDTKick\n\r");
	printf("\t 5) Test SaveSatTimeInFRAM\n\r");
	printf("\t 6) Maintenance Main Loop\n\r");

	unsigned int number_of_tests = 8;
	while (UTIL_DbguGetIntegerMinMax(&selection, 0, number_of_tests) == 0);

	switch (selection)
	{
	case 0:
		offerMoreTests = FALSE;
		break;
	case 1:
		offerMoreTests = FALSE;
		break;
	case 2:
		offerMoreTests = FALSE;
		break;
	case 3:
		offerMoreTests = FALSE;
		break;
	case 4:
		offerMoreTests = FALSE;
		break;
	case 5:
		offerMoreTests = FALSE;
		break;
	case 6:
		offerMoreTests = FALSE;
		break;
	case 7:
		offerMoreTests = FALSE;
		break;
	case 8:
		offerMoreTests = FALSE;
		break;
	default:
		break;
	}
	return offerMoreTests;
}

Boolean MainMaintenanceTestBench()
{
	Boolean offerMoreTests = FALSE;

	while (1) {
		offerMoreTests = selectAndExecuteMaintenanceDemoTest();

		if (offerMoreTests == FALSE) {
			return FALSE;
		}
	}
	return FALSE;
}
