/*
 * EPS_CMD.c
 *
 *  Created on: Jun 13, 2019
 *      Author: Hoopoe3n
 */
#include <freertos/FreeRTOS.h>
#include <hal/errors.h>
#include <hal/Storage/FRAM.h>

#include "EPS_CMD.h"

void cmd_upload_volt_logic(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_UPDATE_EPS_VOLTAGES;
	if (cmd.length != 8 * 2)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	*err = ERR_SUCCESS;
	//convert raw logic to voltage_t[8]
	voltage_t eps_logic[6];
	voltage_t comm_vol[2];
	for(int i = 0; i < 6; i++)
	{
		eps_logic[i] = BigEnE_raw_to_uShort(cmd.data + i*2);
	}
	for(int i = 0; i < 2; i++)
	{
		comm_vol[i] = BigEnE_raw_to_uShort(cmd.data + i*2 + 12);
	}

	// check logic
	if (eps_logic[0] < EPS_VOL_LOGIC_MIN)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	if (eps_logic[5] >= eps_logic[1] || eps_logic[1] >= eps_logic[4])
	{
		*err = ERR_PARAMETERS;
		return;
	}
	if (eps_logic[4] >= eps_logic[2] || eps_logic[2] >= eps_logic[3])
	{
		*err = ERR_PARAMETERS;
		return;
	}
	if (eps_logic[2] >= eps_logic[3] || eps_logic[3] > EPS_VOL_LOGIC_MAX)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	if (eps_logic[1] >= eps_logic[4] || eps_logic[4] >= eps_logic[2])
	{
		*err = ERR_PARAMETERS;
		return;
	}
	if (eps_logic[0] >= eps_logic[5] || eps_logic[5] >= eps_logic[1])
	{
		*err = ERR_PARAMETERS;
		return;
	}

	for (int i = 0; i < 2; i++)
	{
		if (eps_logic[3] < comm_vol[i] && comm_vol[i] < eps_logic[4])
		{
			*err = ERR_PARAMETERS;
			return;
		}
	}

	int FRAM_err = FRAM_writeAndVerify((byte*)eps_logic, EPS_VOLTAGES_ADDR, 12);
	check_int("cmd_upload_volt_logic, FRAM_writeAndVerify(EPS_VOLTAGES_ADDR)", FRAM_err);
	if (FRAM_err)
	{
		*err = ERR_FRAM_WRITE_FAIL;
		return;
	}

	FRAM_err = FRAM_writeAndVerify((byte*)comm_vol, BEACON_LOW_BATTERY_STATE_ADDR, 2);
	check_int("cmd_upload_volt_logic, FRAM_writeAndVerify(BEACON_LOW_BATTERY_STATE_ADDR)", FRAM_err);
	if (FRAM_err)
	{
		reset_EPS_voltages();
		*err = ERR_FRAM_WRITE_FAIL;
		return;
	}

	byte raw[2];
	raw[0] = (byte)(comm_vol[1]);
	raw[1] = (byte)(comm_vol[1] << 8);
	FRAM_err = FRAM_writeAndVerify(raw, TRANS_LOW_BATTERY_STATE_ADDR, 2);
	check_int("cmd_upload_volt_logic, FRAM_writeAndVerify(TRANS_LOW_BATTERY_STATE_ADDR)", FRAM_err);
	if (FRAM_err)
	{
		reset_EPS_voltages();
		*err = ERR_FRAM_WRITE_FAIL;
		return;
	}

	*err = ERR_SUCCESS;
}
void cmd_upload_volt_COMM(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_UPDATE_COMM_VOLTAGES;
	if (cmd.length != 2 * 2)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	voltage_t comm_vol[2];
	voltage_t eps_logic[6];

	for (int i = 0; i < 2; i++)
	{
		comm_vol[i] = BigEnE_raw_to_uShort(cmd.data + i*2);
	}

	int i_error = FRAM_read((byte*)eps_logic, EPS_VOLTAGES_ADDR, 12);
	check_int("cmd_upload_volt_COMM, FRAM_write(EPS_VOLTAGES_ADDR)", i_error);
	for (int i = 0; i < 2; i++)
	{
		if (eps_logic[3] < comm_vol[i] && comm_vol[i] < eps_logic[4])
		{
			*err = ERR_PARAMETERS;
			return;
		}
	}

	i_error = FRAM_write((byte*)comm_vol, BEACON_LOW_BATTERY_STATE_ADDR, 2);
	check_int("cmd_upload_volt_COMM, FRAM_write(BEACON_LOW_BATTERY_STATE_ADDR)", i_error);
	voltage_t volll = comm_vol[1];
	i_error = FRAM_write((byte*)&volll, TRANS_LOW_BATTERY_STATE_ADDR, 2);
	check_int("cmd_upload_volt_COMM, FRAM_write(BEACON_LOW_BATTERY_STATE_ADDR)", i_error);
}
void cmd_heater_temp(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	// 1. define type for later ACK
	*type = ACK_UPDATE_EPS_HEATER_VALUES;
	if (cmd.length != 2)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	// 2. sets values in eps_config_t
    eps_config_t config_data;
    config_data.fields.battheater_low = cmd.data[0];
    config_data.fields.battheater_high = cmd.data[1];
    // 3. sets the new values in the EPS beef(controller)
    int error = GomEpsConfigSet(I2C_BUS_ADDR, &config_data);
    // 4. check for errors
    switch (error)
    {
		case E_NO_SS_ERR:
			*err = ERR_SUCCESS;
			break;
		case E_NOT_INITIALIZED:
			*err = ERR_NOT_INITIALIZED;
			break;
		default:
			*err = ERR_FAIL;
			break;
	}
}
void cmd_SHUT_CAM(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_EPS_SHUT_SYSTEM;
	if (cmd.length != 0)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	*err = ERR_SUCCESS;
	shut_CAM(SWITCH_ON);
}
void cmd_SHUT_ADCS(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_EPS_SHUT_SYSTEM;
	if (cmd.length != 0)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	*err = ERR_SUCCESS;
	shut_ADCS(SWITCH_ON);
}
void cmd_allow_CAM(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_EPS_SHUT_SYSTEM;
	if (cmd.length != 0)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	*err = ERR_SUCCESS;
	shut_CAM(SWITCH_OFF);
}
void cmd_allow_ADCS(Ack_type* type, ERR_type* err, TC_spl cmd)
{
	*type = ACK_EPS_SHUT_SYSTEM;
	if (cmd.length != 0)
	{
		*err = ERR_PARAMETERS;
		return;
	}
	*err = ERR_SUCCESS;
	shut_ADCS(SWITCH_OFF);
}
