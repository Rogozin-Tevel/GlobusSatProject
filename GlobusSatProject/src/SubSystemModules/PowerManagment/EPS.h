/*
 * @file	EPS.h
 * @brief	EPS- Energy Powering System.This system is incharge of the energy consumtion
 * 			the satellite and switching on and off power switches(5V, 3V3)
 * @see		inspect logic flowcharts thoroughly in order to write the code in a clean manner.
 */

#include "GlobalStandards.h"
#include "EPSOperationModes.h"

/*
 	 	 	 	  ______
			  ___|		|___
 	 	 	 |				|
 	 	 	 |	FULL MODE	|
 	 	 	 |- - - - - - -	|	-> FULL UP = 7400
 	 	 	 |- - - - - - - |	-> FULL DOWN = 7300
 	 	 	 |				|
 	 	 	 |	CRUISE MODE	|
 	 	 	 |- - - - - - -	|	-> CRUISE UP = 7200
 	 	 	 |- - - - - - - |	-> CRUISE DOWN = 7100
 	 	 	 |				|
 	 	 	 |	SAFE MODE	|
 	 	 	 |- - - - - - -	| 	-> SAFE UP = 6600
 	 	 	 |- - - - - - - |	-> SAFE DOWN = 6500
 	 	 	 |				|
 	 	 	 |	CRITICAL	|
 	 	 	 |______________|
 */
#define DEFAULT_ALPHA_VALUE 0.95

#define NUMBER_OF_THRESHOLD_VOLTAGES 	6 		///< first 3 are charging voltages, last 3 are discharging voltages
#define DEFAULT_EPS_THRESHOLD_VOLTAGES 	{(voltage_t)6500, (voltage_t)7100, (voltage_t)7300,	 \
										  (voltage_t)6600, (voltage_t)7200, (voltage_t)7400}

#define INDEX_UP_FULL		5
#define INDEX_UP_CRUISE		4
#define INDEX_UP_SAFE		3
#define INDEX_DOWN_FULL		2
#define INDEX_DOWN_CRUISE	1
#define INDEX_DOWN_SAFE		0

/*!
 * @brief initializes the EPS subsystem.
 * @return	0 on success
 * 			-1 on failure of init
 */
int EPS_Init();

/*!
 * @brief EPS logic. controls the state machine of which subsystem
 * is on or off, as a function of only the battery voltage
 * @return	0 on success
 * 			-1 on failure setting state of channels
 */
int EPS_Conditioning();

/*!
 * @brief returns the current voltage on the battery
 * @param[out] vbat he current battery voltage
 * @return	0 on success
 * 			Error code according to <hal/errors.h>
 */
int GetBatteryVoltage(voltage_t *vbat);

/*!
 * @brief setting the new EPS logic threshold voltages on the FRAM.
 * @param[in] thresh_volts an array holding the new threshold values
 * @return	0 on success
 * 			-1 on failure setting new threshold voltages
 * 			-2 on invalid thresholds
 * 			ERR according to <hal/errors.h>
 */
int UpdateThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES]);

/*!
 * @brief getting the EPS logic threshold  voltages on the FRAM.
 * @param[out] thresh_volts a buffer to hold the threshold values
 * @return	0 on success
 * 			-1 on NULL input array
 * 			-2 on FRAM read errors
 */
int GetThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES]);

/*!
 * @brief getting the smoothing factor (alpha) from the FRAM.
 * @param[out] alpha a buffer to hold the smoothing factor
 * @return	0 on success
 * 			-1 on NULL input array
 * 			-2 on FRAM read errors
 */
int GetAlpha(float *alpha);

/*!
 * @brief setting the new voltage smoothing factor (alpha) on the FRAM.
 * @param[in] new_alpha new value for the smoothing factor alpha
 * @note new_alpha is a value in the range - (0,1)
 * @return	0 on success
 * 			-1 on failure setting new smoothing factor
 * 			-2 on invalid alpha
 * @see LPF- Low Pass Filter at wikipedia: https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization
 */
int UpdateAlpha(float new_alpha);

/*!
 * @brief setting the new voltage smoothing factor (alpha) to be the default value.
 * @return	0 on success
 * 			-1 on failure setting new smoothing factor
 * @see DEFAULT_ALPHA_VALUE
 */
int RestoreDefaultAlpha();

/*!
 * @brief	setting the new EPS logic threshold voltages on the FRAM to the default.
 * @return	0 on success
 * 			-1 on failure setting smoothing factor
  * @see EPS_DEFAULT_THRESHOLD_VOLTAGES
 */
int RestoreDefaultThresholdVoltages();


