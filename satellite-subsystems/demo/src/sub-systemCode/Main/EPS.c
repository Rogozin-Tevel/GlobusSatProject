/*
 *
 * EPS.c
 *
 *  Created on: 23  2018
 *      Author: I7COMPUTER
 */
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <at91/utility/exithandler.h>
#include <at91/commons.h>
#include <at91/utility/trace.h>
#include <at91/peripherals/cp15/cp15.h>

#include <hal/Utility/util.h>
#include <hal/Timing/WatchDogTimer.h>
#include <hal/Timing/Time.h>
#include <hal/Drivers/I2C.h>
#include <hal/Drivers/LED.h>
#include <hal/boolean.h>
#include <hal/errors.h>

#include <hal/Storage/FRAM.h>

#include <satellite-subsystems/IsisSolarPanelv2.h>

#include <string.h>
#include "../Global/Global.h"
#include "../Global/GlobalParam.h"
#include "../EPS.h"

#define CAM_STATE	 0x0F
#define ADCS_STATE	 0xF0

#define CALCAVARAGE3(vbatt_prev) (((vbatt_prev)[0] + (vbatt_prev)[1] + (vbatt_prev)[2])/3)
#define CALCAVARAGE2(vbatt_prev, vbatt) ((vbatt_prev + vbatt) / 2)

#define CHECK_CHANNEL_0(preState, currState) ((unsigned char)preState.fields.channel3V3_1 != currState.fields.output[0])
#define CHECK_CHANNEL_3(preState, currState) ((unsigned char)preState.fields.channel5V_1 != currState.fields.output[3])
#define CHECK_CHANNEL_CHANGE(preState, currState) CHECK_CHANNEL_0(preState, currState) || CHECK_CHANNEL_3(preState, currState)

static 	gom_eps_channelstates_t switches_states;

#define DEFULT_VALUES_VOL_TABLE	{ 6600, 7000, 7400, 7500, 7100, 6700}

voltage_t convert_vol(voltage_t vol)
{
	if (vol % 20 > 10)
	{
		return (voltage_t)((double)(vol) / 20) * 20 + 20;
	}
	else
	{
		return (voltage_t)((double)(vol) / 20) * 20;
	}
}

Boolean8bit  get_shut_ADCS()
{
	Boolean8bit mode;
	int error = FRAM_read(&mode, SHUT_ADCS_ADDR, 1);
	check_int("shut_ADCS, FRAM_read", error);
	return mode;
}
void shut_ADCS(Boolean mode)
{
	int error = FRAM_write((byte*)&mode, SHUT_ADCS_ADDR, 1);
	check_int("shut_ADCS, FRAM_write", error);
}

Boolean8bit  get_shut_CAM()
{
	Boolean8bit mode;
	int error = FRAM_read(&mode, SHUT_CAM_ADDR, 1);
	check_int("shut_CAM, FRAM_read", error);
	return mode;
}
void shut_CAM(Boolean mode)
{
	int error = FRAM_write((byte*)&mode, SHUT_CAM_ADDR, 1);
	check_int("shut_CAM, FRAM_write", error);
}

void EPS_Init()
{
	int error = 0;

	// 1. Initialisation by the drivers function
	unsigned char eps_i2c_address = EPS_I2C_ADDR;

	error = GomEpsInitialize(&eps_i2c_address, 1);
	if(0 != error)
	{
		printf("error in GomEpsInitialize = %d\n",error);
	}

	//init solar panels
	error = IsisSolarPanelv2_initialize(slave0_spi);
	check_int("EPS_Init, IsisSolarPanelv2_initialize", error);
	IsisSolarPanelv2_sleep();
	// 2. Housekeeping update for the initial power conditioning
	gom_eps_hk_t eps_tlm;
	GomEpsGetHkData_general(0, &eps_tlm);

	voltage_t current_vbatt = eps_tlm.fields.vbatt;
	current_vbatt = convert_vol(current_vbatt);

	// 3. Obtaining of the constant voltage values of the states limits (according to the EPS software requirements document)
	voltage_t voltages[EPS_VOLTAGES_SIZE] = DEFULT_VALUES_VOL_TABLE;
	//FRAM_read(raw_voltage, EPS_VOLTAGES_ADDR, EPS_VOLTAGES_SIZE_RAW); // Addresses are temporary and took from the Hoopoe code (until we organize the FRAM)
	//convert_raw_voltage(raw_voltage, voltages);
	// 4. Initial power conditioning
	if (current_vbatt < voltages[0])
	{
		// Enter critical mode - ADCS actuators are shut down as well
		EnterCriticalMode(&switches_states);
	}
	else if (current_vbatt  < voltages[1])
	{
		// Enter safe mode - transmitter is shut down as well
		EnterSafeMode(&switches_states);
	}
	else if (current_vbatt  < voltages[2])
	{
		// Enter cruise mode - payload is shut down
		EnterCruiseMode(&switches_states);
	}
	else
	{
		// Enter full mode - everything is on
		EnterFullMode(&switches_states);
	}

	// 5. Set the switches output accordingly to the decided ones
	error = GomEpsSetOutput(0, switches_states);
	check_int("EPS_Init, GomEpsSetOutput", error);

	// 6. Initialize the current vbatt and the previous vbatts
	voltage_t pre_vbatt[3];
	pre_vbatt[0] = current_vbatt;
	pre_vbatt[1] = current_vbatt;
	pre_vbatt[2] = current_vbatt;
	set_Vbatt_previous(pre_vbatt);
	set_Vbatt(current_vbatt);

	// 7. Save the states on the FRAM for the continuation of the logic
	error = FRAM_write(&switches_states.raw, EPS_STATES_ADDR, 1);
	check_int("EPS_init, FRAM_write", error);
}

void reset_FRAM_EPS()
{
	int i_error;

	reset_EPS_voltages();

	//reset EPS_STATES_ADDR
	byte data = 0;
	i_error = FRAM_write(&data, EPS_STATES_ADDR, 1);
	check_int("reset_FRAM_EPS, FRAM_write", i_error);
	data = SWITCH_OFF;
	i_error = FRAM_write(&data, SHUT_ADCS_ADDR, 1);
	check_int("reset_FRAM_EPS, FRAM_write", i_error);
	i_error = FRAM_write(&data, SHUT_CAM_ADDR, 1);
	check_int("reset_FRAM_EPS, FRAM_write", i_error);
}

void reset_EPS_voltages()
{
	int i_error;
	//reset EPS_VOLTAGES_ADDR
	voltage_t voltages[EPS_VOLTAGES_SIZE] = DEFULT_VALUES_VOL_TABLE;
	voltage_t comm_voltage  = 7250;

	i_error = FRAM_write((byte*)voltages, EPS_VOLTAGES_ADDR, EPS_VOLTAGES_SIZE_RAW);
	check_int("reset_FRAM_EPS, FRAM_read", i_error);

	i_error = FRAM_write((byte*)&comm_voltage, BEACON_LOW_BATTERY_STATE_ADDR, 2);
	check_int("reset_FRAM_EPS, FRAM_read", i_error);

	i_error = FRAM_write((byte*)&comm_voltage, TRANS_LOW_BATTERY_STATE_ADDR, 2);
	check_int("reset_FRAM_EPS, FRAM_read", i_error);
}

void EPS_Conditioning()
{
	int i_error;
	voltage_t voltages[EPS_VOLTAGES_SIZE] = DEFULT_VALUES_VOL_TABLE;// Remove magical numbers and declare a pointer when the FRAM will be organised

	voltage_t vbatt_prev[3];
	get_Vbatt_previous(vbatt_prev);

	gom_eps_hk_t eps_tlm;
	i_error = GomEpsGetHkData_general(0, &eps_tlm);
	check_int("can't get gom_eps_hk_t for vBatt in EPS_Conditioning", i_error);
	if (i_error != 0)
		return;

	voltage_t vbatt_filtered = CALCAVARAGE3(vbatt_prev);
	voltage_t currentvbatt = eps_tlm.fields.vbatt;

	currentvbatt = CALCAVARAGE2(vbatt_filtered, currentvbatt);

	vbatt_filtered = convert_vol(vbatt_filtered);;
	currentvbatt = convert_vol(currentvbatt);

	set_Vbatt(currentvbatt);
	//printf("Current Vbatt: %d, Vbatt_pre: %d ", currentvbatt, vbatt_filtered);

	byte raw_vol_fram[EPS_VOLTAGES_SIZE_RAW];
	FRAM_read(raw_vol_fram, EPS_VOLTAGES_ADDR, EPS_VOLTAGES_SIZE_RAW);
	convert_raw_voltage(raw_vol_fram, voltages);

	/*printf("channel 1: %d, channe: %d\n", eps_tlm.fields.output[0], eps_tlm.fields.output[3]);
	printf("current voltage: %u, previous voltage: %u\n\n", currentvbatt, vbatt_filtered);
	printf("ADCS: %d\n", get_system_state(ADCS_param));
	printf("cam: %d\n", get_system_state(cam_operational_param));
	printf("TX: %d\n", get_system_state(Tx_param));*/
	if (vbatt_filtered > currentvbatt)
	{
		printf(". downward ");
		if (currentvbatt < voltages[0])
		{
			EnterCriticalMode(&switches_states);
		}
		else if (currentvbatt < voltages[1])
		{
			EnterSafeMode(&switches_states);
		}
		else if (currentvbatt < voltages[2])
		{
			EnterCruiseMode(&switches_states);
		}
		else
		{
			EnterFullMode(&switches_states);
		}
	}
	// Conditioning on upward trend of battery voltage
	else if (vbatt_filtered < currentvbatt)
	{
		printf(". upward ");
		if (currentvbatt > voltages[3])
		{
			EnterFullMode(&switches_states);
		}
		else if (currentvbatt > voltages[4])
		{
			EnterCruiseMode(&switches_states);
		}
		else if (currentvbatt > voltages[5])
		{
			EnterSafeMode(&switches_states);
		}
		else
		{
			EnterCriticalMode(&switches_states);
		}
	}
	else
	{
		return;
	}

	printf("\n");

	if (CHECK_CHANNEL_CHANGE(switches_states, eps_tlm))
	{
		i_error = GomEpsSetOutput(0, switches_states);
		check_int("can't set channel state in EPS_Conditioning", i_error);

		i_error = FRAM_write(&switches_states.raw, EPS_STATES_ADDR, 1);
		check_int("EPS_Conditioning, FRAM_write", i_error);
	}
}

Boolean overRide_ADCS(gom_eps_channelstates_t* switches_states)
{
	if(get_shut_ADCS())
	{
		switches_states->raw = 0;
		set_system_state(ADCS_param, SWITCH_OFF);

		return FALSE;
	}

	return TRUE;
}

Boolean overRide_Camera()
{
	if(get_shut_CAM())
	{
		set_system_state(cam_operational_param, SWITCH_OFF);

		return FALSE;
	}

	return TRUE;
}

//EPS modes
void EnterFullMode(gom_eps_channelstates_t* switches_states)
{
	printf("Enter Full Mode\n");
	set_system_state(Tx_param, SWITCH_ON);

	if (overRide_ADCS(switches_states))
	{
		switches_states->fields.quadbatSwitch = 0;
		switches_states->fields.quadbatHeater = 0;
		switches_states->fields.channel3V3_1 = 1;
		switches_states->fields.channel3V3_2 = 0;
		switches_states->fields.channel3V3_3 = 0;
		switches_states->fields.channel5V_1 = 1;
		switches_states->fields.channel5V_2 = 0;
		switches_states->fields.channel5V_3 = 0;

		set_system_state(ADCS_param, SWITCH_ON);
	}

	if (overRide_Camera())
		set_system_state(cam_operational_param, SWITCH_ON);
}

void EnterCruiseMode(gom_eps_channelstates_t* switches_states)
{
	printf("Enter Cruise Mode\n");
	set_system_state(Tx_param, SWITCH_ON);

	if (overRide_ADCS(switches_states))
	{
		switches_states->fields.quadbatSwitch = 0;
		switches_states->fields.quadbatHeater = 0;
		switches_states->fields.channel3V3_1 = 1;
		switches_states->fields.channel3V3_2 = 0;
		switches_states->fields.channel3V3_3 = 0;
		switches_states->fields.channel5V_1 = 1;
		switches_states->fields.channel5V_2 = 0;
		switches_states->fields.channel5V_3 = 0;

		set_system_state(ADCS_param, SWITCH_ON);
	}

	set_system_state(cam_operational_param, SWITCH_OFF);
}

void EnterSafeMode(gom_eps_channelstates_t* switches_states)
{
	printf("Enter Safe Mode\n");
	set_system_state(Tx_param, SWITCH_OFF);

	if (overRide_ADCS(switches_states))
	{
		switches_states->fields.quadbatSwitch = 0;
		switches_states->fields.quadbatHeater = 0;
		switches_states->fields.channel3V3_1 = 1;
		switches_states->fields.channel3V3_2 = 0;
		switches_states->fields.channel3V3_3 = 0;
		switches_states->fields.channel5V_1 = 1;
		switches_states->fields.channel5V_2 = 0;
		switches_states->fields.channel5V_3 = 0;

		set_system_state(ADCS_param, SWITCH_ON);
	}

	set_system_state(cam_operational_param, SWITCH_OFF);
}

void EnterCriticalMode(gom_eps_channelstates_t* switches_states)
{
	printf("Enter Critical Mode\n");
	switches_states->fields.quadbatSwitch = 0;
	switches_states->fields.quadbatHeater = 0;
	switches_states->fields.channel3V3_1 = 0;
	switches_states->fields.channel3V3_2 = 0;
	switches_states->fields.channel3V3_3 = 0;
	switches_states->fields.channel5V_1 = 0;
	switches_states->fields.channel5V_2 = 0;
	switches_states->fields.channel5V_3 = 0;

	set_system_state(cam_operational_param, SWITCH_OFF);
	set_system_state(Tx_param, SWITCH_OFF);
	set_system_state(ADCS_param, SWITCH_OFF);
}

//Write gom_eps_k_t
void WriteCurrentTelemetry(gom_eps_hk_t telemetry)
{
	/*printf("vbatt: %d\nvboost: %d, %d, %d\ncurin: %d, %d, %d\ntemp: %d, %d, %d, %d, %d, %d\ncursys: %d\ncursun: %d\nstates: %d\n",
		telemetry.fields.vbatt, telemetry.fields.vboost[0], telemetry.fields.vboost[1], telemetry.fields.vboost[2],
		telemetry.fields.curin[0], telemetry.fields.curin[1], telemetry.fields.curin[2],
		telemetry.fields.temp[0], telemetry.fields.temp[1], telemetry.fields.temp[2], telemetry.fields.temp[3], telemetry.fields.temp[4], telemetry.fields.temp[5],
		telemetry.fields.cursys, telemetry.fields.cursun, states);*/

	printf("Battery Voltage: %d mV\n", telemetry.fields.vbatt);
	printf("Boost Converters: %d, %d, %d mV\n", telemetry.fields.vboost[0], telemetry.fields.vboost[1], telemetry.fields.vboost[2]);
	printf("Currents: %d, %d, %d mA\n", telemetry.fields.curin[0], telemetry.fields.curin[1], telemetry.fields.curin[2]);
	printf("Temperatures: %d, %d, %d, %d, %d, %d C\n", telemetry.fields.temp[0], telemetry.fields.temp[1], telemetry.fields.temp[2], telemetry.fields.temp[3], telemetry.fields.temp[4], telemetry.fields.temp[5]);
	printf("Current out of Battery: %d mA\n", telemetry.fields.cursys);
	printf("Current of Sun Sensor: %d mA\n", telemetry.fields.cursun);
}

void convert_raw_voltage(byte raw[EPS_VOLTAGES_SIZE_RAW], voltage_t voltages[EPS_VOLTAGES_SIZE])
{
	int i;
	int l = 0;
	for (i = 0; i < EPS_VOLTAGES_SIZE; i++)
	{
		voltages[i] = (voltage_t)raw[l];
		l++;
		voltages[i] += (voltage_t)(raw[l] << 8);
		l++;
	}
}
