#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "../Ants.h"

#include <hal/Storage/FRAM.h>
#include <hal/errors.h>
#include <stdio.h>

#include <satellite-subsystems/GomEPS.h>

void init_Ants()
{
    int retValInt = 0;

	ISISantsI2Caddress myAntennaAddress[2];
	myAntennaAddress[0].addressSideA = 0x31;
	myAntennaAddress[0].addressSideB = 0x32;

	//Initialize the AntS system
	retValInt = IsisAntS_initialize(myAntennaAddress, 1);
	check_int("init_Ants, IsisAntS_initialize", retValInt);
}

int ARM_ants()
{
	int error;
	//2. changing state in side A
	ISISantsSide side = isisants_sideA;
	error = IsisAntS_setArmStatus(I2C_BUS_ADDR, side, isisants_arm);
	check_int("ARM_ants, IsisAntS_setArmStatus", error);
	if (error == E_NOT_INITIALIZED)
	{
		return -2;
	}
	else if(error != 0)
	{
		return -1;
	}
	//3. changing state in side B
	side = isisants_sideB;
	IsisAntS_setArmStatus(I2C_BUS_ADDR, side, isisants_arm);
	if (error == E_NOT_INITIALIZED)
	{
		return -2;
	}
	else if(error != 0)
	{
		return -1;
	}

	return 0;
}

int DISARM_ants()
{
	int error;
	//2. changing state in side A
	ISISantsSide side = isisants_sideA;
	error = IsisAntS_setArmStatus(I2C_BUS_ADDR, side, isisants_disarm);
	check_int("ARM_ants, IsisAntS_setArmStatus", error);
	if (error == E_NOT_INITIALIZED)
	{
		return -2;
	}
	else if(error != 0)
	{
		return -1;
	}
	//3. changing state in side B
	side = isisants_sideB;
	IsisAntS_setArmStatus(I2C_BUS_ADDR, side, isisants_disarm);
	if (error == E_NOT_INITIALIZED)
	{
		return -2;
	}
	else if(error != 0)
	{
		return -1;
	}

	return 0;
}

int deploye_ants(ISISantsSide side)
{
	(void)side;
#ifndef ANTS_DO_NOT_DEPLOY
	int error = ARM_ants();

	if (error != 0)
	{
		printf("error in Arm sequence: %d\n", error);
		return error;
	}

	error = IsisAntS_autoDeployment(0, side, DEFFULT_DEPLOY_TIME);
	check_int("IsisAntS_autoDeployment, side A", error);

	return 0;
#else
	printf("Are you crazy, please do NOT try to deploy ants in the lab\n");
	return 666;
#endif
}

int time_out_before_deploy(int attempt_number)
{
	if (attempt_number > 2)
		return -1;

	int i_error;
	deploy_attempt deploy_status;
	i_error = FRAM_read((byte*)&deploy_status, DEPLOY_ANTS_ATTEMPTS_ADDR + attempt_number, 1);
	check_int("FRAM_read(DEPLOY_ANTS_ATTEMPTS_ADDR), time_out_before_deploy", i_error);
	if (deploy_status.isAtemptDone)
		return 1;

	gom_eps_hk_t eps_tlm;
	portTickType xLastWakeTime = xTaskGetTickCount();
	const portTickType xFrequency = ONE_MINUTE;

	while (deploy_status.minutesToAttempt < START_MUTE_TIME_MIN)
	{
		i_error = GomEpsGetHkData_general(0, &eps_tlm);
		check_int("can't get gom_eps_hk_t for vBatt in EPS_Conditioning", i_error);

		deploy_status.minutesToAttempt++;
		i_error = FRAM_write((byte*)&deploy_status, DEPLOY_ANTS_ATTEMPTS_ADDR + attempt_number, 1);
		check_int("FRAM_write(DEPLOY_ANTS_ATTEMPTS_ADDR), time_out_before_deploy", i_error);

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		printf("%d minutes to deploy!\n", START_MUTE_TIME_MIN - deploy_status.minutesToAttempt);
	}

	return 0;
}

void Auto_Deploy()
{
	deploy_attempt deploy_status;
	int i_error;

	if (time_out_before_deploy(DEPLOY_ATTEMPT_NUMBER_1) == 0)
	{
		printf("starts deployment 1, side A\n");
		deploye_ants(isisants_sideA);

		deploy_status.isAtemptDone = 1;
		deploy_status.minutesToAttempt = START_MUTE_TIME_MIN;
		i_error = FRAM_write((byte*)&deploy_status, DEPLOY_ANTS_ATTEMPTS_ADDR + DEPLOY_ATTEMPT_NUMBER_1, 1);
		check_int("FRAM_write(DEPLOY_ANTS_ATTEMPTS_ADDR), time_out_before_deploy", i_error);
		printf("ended attempt 1 to deploy ants, side A\n");
	}

	/*if (time_out_before_deploy(DEPLOY_ATTEMPT_NUMBER_1) == 0)
	{
		printf("starts deployment 2, side B\n");
		deploye_ants(isisants_sideB);

		deploy_status.isAtemptDone = TRUE;
		deploy_status.minutesToAttempt = START_MUTE_TIME_MIN;
		i_error = FRAM_write(&deploy_status, DEPLOY_ANTS_ATTEMPTS_ADDR + DEPLOY_ATTEMPT_NUMBER_2, 1);
		check_int("FRAM_write(DEPLOY_ANTS_ATTEMPTS_ADDR), time_out_before_deploy", i_error);
		printf("ended attempt 2 to deploy ants, side B\n");
	}

	if (time_out_before_deploy(DEPLOY_ATTEMPT_NUMBER_1) == 0)
	{
		printf("starts deployment 3, side A\n");
		deploye_ants(isisants_sideA);

		deploy_status.isAtemptDone = TRUE;
		deploy_status.minutesToAttempt = START_MUTE_TIME_MIN;
		i_error = FRAM_write(&deploy_status, DEPLOY_ANTS_ATTEMPTS_ADDR + DEPLOY_ATTEMPT_NUMBER_3, 1);
		check_int("FRAM_write(DEPLOY_ANTS_ATTEMPTS_ADDR), time_out_before_deploy", i_error);
		printf("ended attempt 3 to deploy ants, side A\n");
	}*/
}
