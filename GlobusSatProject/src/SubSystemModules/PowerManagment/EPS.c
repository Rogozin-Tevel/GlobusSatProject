#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <hal/errors.h>

#include <string.h>

#include "EPS.h"
#ifdef ISISEPS
#include <satellite-subsystems/IsisEPS.h>
#endif // ISISEPS

#ifdef GOMEPS
#include <satellite-subsystems/GomEPS.h>
#endif // GOMEPS

// TODO: What does this documentation mean?
//		 And why doesn't it match the code?

// y[i] = a * x[i] +(1-a) * y[i-1]
voltage_t prev_avg = 0;		// y[i-1]
float     alpha    = 0;		//<! smoothing constant

voltage_t eps_threshold_voltages[NUMBER_OF_THRESHOLD_VOLTAGES];	// saves the current EPS logic threshold voltages

int GetBatteryVoltage(voltage_t *vbatt)
{ // Done
	ieps_enghk_data_cdb_t p_raenghk_data_cdb;

	ieps_statcmd_t p_rsp_code;

	unsigned char index = EPS_I2C_BUS_INDEX;

	ieps_board_t board = ieps_board_cdb1;

	// Get house keeping data
	int err = IsisEPS_getRAEngHKDataCDB(index, board, &p_raenghk_data_cdb, &p_rsp_code);

	*vbatt = p_raenghk_data_cdb.fields.bat_voltage; // Save the batt voltage

	if (err != E_NO_SS_ERR)
	{
	    return 0;
	}
	return -1;
}

int EPS_Init()
{ // Done
    unsigned char eps_addr = EPS_I2C_ADDR;

	int err_code = IsisEPS_initialize(&eps_addr, 1);

	if (err_code != E_NO_SS_ERR)
	{
		return -1;
	}

	err_code = IsisSolarPanelv2_initialize(slave0_spi);

	if (err_code == ISIS_SOLAR_PANEL_STATE_NOINIT)
	{
		return -2;
	}

	IsisSolarPanelv2_sleep(); // Reduce the power consumption

	err_code = GetThresholdVoltages(eps_threshold_voltages);
	if (err_code != 0)
	{
		voltage_t temp[] = DEFAULT_EPS_THRESHOLD_VOLTAGES;
		memcpy(eps_threshold_voltages, temp, sizeof(temp));
		return -3;
	}

	err_code = GetAlpha(&alpha);
	if (err_code != 0)
	{
		alpha = DEFAULT_ALPHA_VALUE;
		return -4;
	}

	GetBatteryVoltage(&prev_avg);
    EPS_Conditioning();

    return 0;
}

int EPS_Conditioning()
{ // Done?
	//******done by diana*****
	// Fixed by Maor & Yanir

	voltage_t vbatt; 	// Battery voltage
	voltage_t curr_avg;	// TODO: What is this? current average? average of what?

	int err_code = GetBatteryVoltage(&vbatt);

	if (err_code != 0)
	{
		printf("Error when getting battery voltages!\n");
		return -1;
	}

	err_code = GetAlpha(&alpha);

	if (err_code != 0)
	{
		printf("Error on EPS_Conditioning: Can't read alpha!\n");
		return -1;
	}

	err_code = GetThresholdVoltages(eps_threshold_voltages); // Add the levels to EPS
	if (err_code != 0)
	{
		printf("Error on EPS_Conditioning: Can't load threshold voltages!\n");
		return -1;
	}

	// TODO: This calculation doesn't match the documentation!
	curr_avg = alpha * prev_avg + (1 - alpha) * vbatt; // TODO: ???

	const Boolean is_charging = curr_avg >= prev_avg;

	const EpsState_t prev_state = GetSystemState();

	if (is_charging)
	{
		if (curr_avg >= eps_threshold_voltages[INDEX_UP_FULL])
		{
			if (prev_state != FullMode)
			{
				err_code = EnterFullMode();
				if (err_code != 0)
				{
					printf("Error on EPS_Conditioning: Can't enter full mode!\n");
					return -1;
				}
			}
		}
		else if (curr_avg >= eps_threshold_voltages[INDEX_UP_CRUISE])
		{
			if (prev_state != CruiseMode)
			{
				err_code = EnterCruiseMode();
				if (err_code != 0)
				{
					printf("Error on EPS_Conditioning: Can't enter cruise mode!\n");
					return -1;
				}
			}
		}
		else if (curr_avg >= eps_threshold_voltages[INDEX_UP_SAFE])
		{
			if (prev_state != SafeMode)
			{
				err_code = EnterSafeMode();
				if (err_code != 0)
				{
					printf("Error on EPS_Conditioning: Can't enter safe mode!\n");
					return -1;
				}
			}
		}
	}
	else // The battery isn't charging ATM!
	{
		if (curr_avg <= eps_threshold_voltages[INDEX_DOWN_SAFE])
		{
			if (prev_state != CriticalMode)
			{
				err_code = EnterCriticalMode();
				if (err_code != 0)
				{
					printf("Error on EPS_Conditioning: Can't enter critical mode!\n");
					return -1;
				}
			}
		}
		else if (curr_avg <= eps_threshold_voltages[INDEX_DOWN_CRUISE])
		{
			if (prev_state != SafeMode)
			{
				err_code = EnterSafeMode();
				if (err_code != 0)
				{
					printf("Error on EPS_Conditioning: Can't enter safe mode!\n");
					return -1;
				}
			}
		}
		else if (curr_avg <= eps_threshold_voltages[INDEX_DOWN_FULL])
		{
			if (prev_state != CruiseMode)
			{
				err_code = EnterCruiseMode();
				if (err_code != 0)
				{
					printf("Error on EPS_Conditioning: Can't enter cruise mode!\n");
					return -1;
				}
			}
		}
	}

	prev_avg = curr_avg;

	return 0;
}

int UpdateAlpha(float new_alpha)
{
	//******done by diana*****
	// Fixed by Maor & Yanir

    if (new_alpha < 0 || new_alpha > 1)
    {
    	printf("Error on UpdateAlpha: Alpha is out of range!\n");
    	return -2;
    }

    if (FRAM_write((unsigned char *)&new_alpha, EPS_ALPHA_FILTER_VALUE_ADDR, EPS_ALPHA_FILTER_VALUE_SIZE) != 0)
    {
    	printf("Error on UpdateAlpha: Can't write alpha to FRAM!\n");
    	return -1;
    }

    return 0;
}

int UpdateThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES])
{
	return FRAM_write((unsigned char *)thresh_volts, EPS_THRESH_VOLTAGES_ADDR, EPS_THRESH_VOLTAGES_SIZE);
}

int GetThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES])
{
	return FRAM_read((unsigned char *)thresh_volts, EPS_THRESH_VOLTAGES_ADDR, EPS_THRESH_VOLTAGES_SIZE);
}

int GetAlpha(float *alpha)
{
	int an = FRAM_read((unsigned char *)&alpha, EPS_ALPHA_FILTER_VALUE_ADDR, EPS_ALPHA_FILTER_VALUE_SIZE);

	if (an == -1)
	{
		printf("Error on GetAlpha: Can't obtain lock for FRAM access!\n");
	  	return -2;
	}

	if (an == -2)
	{
		printf("Error on GetAlpha: NULL input array!\n");
		return -1;
	}

	return 0;
}

int RestoreDefaultAlpha()
{
	return UpdateAlpha(DEFAULT_ALPHA_VALUE);
}

int RestoreDefaultThresholdVoltages()
{
	voltage_t temp[NUMBER_OF_THRESHOLD_VOLTAGES] = DEFAULT_EPS_THRESHOLD_VOLTAGES;
	FRAM_write((unsigned char *)temp, EPS_THRESH_VOLTAGES_ADDR, EPS_THRESH_VOLTAGES_SIZE);
	return 0;
}

