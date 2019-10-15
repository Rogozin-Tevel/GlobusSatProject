#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "EpsTestingDemo.h"

#include <hal/Utility/util.h>

#include <SubSystemModules/PowerManagment/EPS.h>
#include <SubSystemModules/PowerManagment/EPSOperationModes.h>
#include <stdlib.h>

#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif

Boolean TestEpsConditioning()
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

	voltage_t vbat = 0;
	while( curr_time < end_time)
	{
		curr_time = xTaskGetTickCount();
		printf("seconds till finish: %d\n",(int)(end_time - curr_time)/1000);
		GetBatteryVoltage(&vbat);
		printf("Vbat = %d\n",vbat);
		printf("channel state = %X\n",GetSystemChannelState());

		switch(GetSystemState()){
			case FullMode:
				printf("\t\n----------Eps is in Full Mode\n\n");
				break;
			case CruiseMode:
				printf("\t\n----------Eps is in Cruise Mode\n\n");
				break;
			case SafeMode:
				printf("\t\n----------Eps is in Safe Mode\n\n");
				break;
			case CriticalMode:
				printf("\t\n----------Eps is in Critical Mode\n\n");
				break;
		}
		for(int i = 0; i < 4; i++)
		{
			EPS_Conditioning();
			vTaskDelay(1000);
		}


	}
	return TRUE;
}

Boolean TestGetBatteryVoltage()
{
	voltage_t vbat = 0;
	int err = GetBatteryVoltage(&vbat);
	if(0!=err){
		printf("error in 'GetBatteryVoltage' = %d",err);
	}
	printf("battery voltage = %d\n",vbat);
	return TRUE;
}

Boolean TestUpdateThreshVoltages()
{
	printf("Updating threshold voltages value\n");

	voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES] = {6501,7101,7301,7401,7201,6601};

	int err = 0;
	printf("Updating threshold voltage array to:");
	for(int i = 0; i <NUMBER_OF_THRESHOLD_VOLTAGES; i++)
	{
		printf("thresh[%d] = %d\n",i,thresh_volts[i]);

	}

	err = UpdateThresholdVoltages(thresh_volts);
	if(0 != err){
		printf("error in 'UpdateThresholdVoltages' = %d\n",err);
		return TRUE;
	}

	err = GetThresholdVoltages(thresh_volts);
	if(0 != err){
		printf("error in 'GetThresholdVoltages' = %d\n",err);
		return TRUE;
	}

	for(int i = 0; i <NUMBER_OF_THRESHOLD_VOLTAGES; i ++)
	{
		printf("threshVolt[%d] = %d\n",i,thresh_volts[i]);
	}
	return TRUE;
}

Boolean TestGetThreshVoltages()
{
	printf("Get threshold voltages value\n");
	voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES];

	int err = GetThresholdVoltages(thresh_volts);
	if(0 != err){
		printf("error in 'GetThresholdVoltages' = %d\n",err);
		return TRUE;
	}
	for(int i = 0; i <NUMBER_OF_THRESHOLD_VOLTAGES; i ++)
	{
		printf("threshVolt[%d] = %d\n",i,thresh_volts[i]);
	}
	return TRUE;
}

Boolean TestUpdateAlpha()
{
	int err = 0;
	printf("Updating Alpha to be '0.66666'");
	float alpha = 0.66666;

	err = UpdateAlpha(alpha);
	if(err != 0){
		printf("Error in 'UpdateAlpha' = %d\n",err);
		return TRUE;
	}

	err = GetAlpha(&alpha);
	if(err != 0){
		printf("Error in 'GetAlpha' = %d\n",err);
		return TRUE;
	}
	if(0 == err){
		printf("Successfully read Alpha value using 'GetAlpha', alpha = %f",alpha);
	}
	return TRUE;
}

Boolean TestGetAlpha()
{
	float alpha = 0;
	int err = GetAlpha(&alpha);
	if(err != 0){
		printf("error in 'GetAlpha' = %d\n",err);
		return TRUE;
	}
	printf("Alpha value = %f\n",alpha);
	return TRUE;
}

Boolean TestRestoreDefaultAlpha()
{
	printf("Restoring default alpha value\n");
	float alpha = 0.0;
	int err = GetAlpha(&alpha);
	if(0 != err){
		printf("error in 'GetAlpha' = %d\n",err);
		return TRUE;
	}
	printf("old alpha value: a = %f\n",alpha);

	err = RestoreDefaultAlpha();
	if(0 != err){
		printf("error in 'RestoreDefaultAlpha' = %d\n",err);
	}
	err = GetAlpha(&alpha);
	if(0 != err){
		printf("error in 'GetAlhpa' = %d\n",err);
		return TRUE;
	}
	printf("new alpha value: a = %f\n",alpha);
	return TRUE;
}

Boolean TestRestoreDefaultThresholdVoltages()
{
	printf("Restoring default threshold voltages value\n");
	voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES];

	int err = GetThresholdVoltages(thresh_volts);
	if(0 != err){
		printf("error in 'GetThresholdVoltages' = %d\n",err);
		return TRUE;
	}
	for(int i = 0; i <NUMBER_OF_THRESHOLD_VOLTAGES; i ++)
	{
		printf("threshVolt[%d] = %d\n",i,thresh_volts[i]);
	}

	err = RestoreDefaultThresholdVoltages();
	if(0 != err){
		printf("error in 'RestoreDefaultThresholdVoltages' = %d\n",err);
	}

	err = GetThresholdVoltages(thresh_volts);
	if(0 != err){
		printf("error in 'GetThresholdVoltages' = %d\n",err);
		return TRUE;
	}

	for(int i = 0; i <NUMBER_OF_THRESHOLD_VOLTAGES; i ++)
	{
		printf("threshVolt[%d] = %d\n",i,thresh_volts[i]);
	}
	return TRUE;
}

Boolean selectAndExecuteEpsDemoTest()
{
	unsigned int selection = 0;
	Boolean offerMoreTests = TRUE;

	printf( "\n\r Select a test to perform: \n\r");
	printf("\t 0) Return to main menu \n\r");
	printf("\t 1) Eps Conditioning \n\r");
	printf("\t 2) Get Battery Voltage \n\r");
	printf("\t 3) Update Threshold Voltages \n\r");
	printf("\t 4) Print Threshold Voltages \n\r");
	printf("\t 5) Update Alpha  \n\r");
	printf("\t 6) Print Alpha  \n\r");
	printf("\t 7) Restore Default Alpha \n\r");
	printf("\t 8) Restore Default Threshold Voltages \n\r");


	unsigned int number_of_tests = 8;
	while(UTIL_DbguGetIntegerMinMax(&selection, 0, number_of_tests) == 0);

	switch(selection) {
	case 0:
		offerMoreTests = FALSE;
		break;
	case 1:
		offerMoreTests = TestEpsConditioning();
		break;
	case 2:
		offerMoreTests = TestGetBatteryVoltage();
		break;
	case 3:
		offerMoreTests = TestUpdateThreshVoltages();
		break;
	case 4:
		offerMoreTests = TestGetThreshVoltages();
		break;
	case 5:
		offerMoreTests = TestUpdateAlpha();
		break;
	case 6:
		offerMoreTests = TestGetAlpha();
		break;
	case 7:
		offerMoreTests = TestRestoreDefaultAlpha();
		break;
	case 8:
		offerMoreTests = TestRestoreDefaultThresholdVoltages();
		break;

	default:
		break;
	}
	return offerMoreTests;
}

Boolean MainEpsTestBench()
{
	Boolean offerMoreTests = FALSE;

	while(1)
	{
		offerMoreTests = selectAndExecuteEpsDemoTest();

		if(offerMoreTests == FALSE)
		{
			return FALSE;
		}
	}
	return FALSE;
}
