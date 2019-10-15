/*
 * StageTable.c
 *
 *  Created on: 22 Mar 2011
 *     @Author Michael
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <stdio.h>
#include <stdlib.h>

#include <satellite-subsystems/cspaceADCS.h>
#include <satellite-subsystems/cspaceADCS_types.h>

#include "Stage_Table.h"
#include "../Global/GlobalParam.h"
#include <hal/Storage/FRAM.h>


//! CubeADCS ID
#define ADCS_ID 0
//! starting run mode of the ADCS
#define START_ADCS_RUN_MODE 1
//! True  == 1 True in c
//#define TRUE 1
//! False == 0 False in c
#define ZERO_ADCS 0
//! Starting Estimation mode of the stage table
#define START_ESTIMATION_MODE 0
//! Starting Delay of the stage table
#define START_DELAY 1000
//! Starting control Mode of the stage table
#define START_CONTROL_MODE 0
//! If We are in Test Mode

//! Get Bit number x From byte
#define GET_BIT_X(Byte,x) (((Byte) >> (x - 1)) & 0B00000001)
//! contains the Fram address for the control mode timeout
#define TIMEOUT_CONTROL_MODE_FRAM_ADDR 0
//! contains the override flag
#define OVERRIDEFLAG_FRAN_ADDR 0
//! the flag size
#define OVERIDE_FLAG_SIZE 1
//! the TIMEOUT size
#define TIMEOUT_FLAG_SIZE 1
//!size of the stage table delay
#define SIZE_OF_DELAY 3
//!size of the stage table telemtry
#define SIZE_OF_TELEMTRY 3
//! the struct satADCSPower contaims the adcs power used in duchifat 3
#define FAST_SPIN_DETUNBLING_SAT 8
#define FAST_SPIN_DETUNBLING_STAGE_TABLE 5
#define VERY_FAST_SPIN_DETUNBLING_SAT 9
#define VERY_FAST_SPIN_DETUNBLING_STAGE_TABLE 6


typedef struct satADCSPower
{
	//! this char contains the 4 powers used in the satt    01010101 1 + 4 + 64
	unsigned char signal_cubecontrol : 2,
	motor_cubecontrol:2,
	pwr_motor:2,
	pwr_cubewheel :2;
}ADCSPower;

//! the union stageTableData that contains the stage table Data.
/*!
 * the union contains an array and a struct to get and set the stage table data
 */
typedef union __attribute__ ((__packed__)) stageTableData
{
	//! an array that holds the adcs data
	unsigned char Raw[9];
	//! The structure Data contains the current stage table Data.
	/*!
	 * the struct as defiened in: https://drive.google.com/open?id=1q-ncHqyy6VxbxmSNS6qITSF_UyRN064tL_WOQUf9ce4
	 */
	 struct __attribute__ ((__packed__)) data
	{
		//! the delay contains the stage table delay parameter that represents the sample rate(the time difference between collecting new data from the cube adcs) of the system
		int delay : 24; //3 bytes
		//! the control mode contains the stage table Control Mode parameter that represents the current control mode of the Cube ADCS
		unsigned char controlMode; //byte1
		//! the power contains the stage table power parameter that represents the current control mode of the Cube ADCS
		ADCSPower powerControl;//byte1
		//! the estimation mode contains the stage table estimation Mode parameter that represents the current control mode of the Cube ADCS
		unsigned char estimationMode; //byte1
		//! the telemtry contains the stage table telemtry parameter that represents what telemtries are being saved on the OBC file system
		TLMData telemtry; // 3 bytes
	} StageData;
}stageTableData;
//! the telemtryStart function returns an initialized struct of the type TLMData
 void telemtryStart(TLMData* telemtry)
{

	// set the EstimatedAttitudeAnglesCheckSaveId146 parameter to false
	telemtry->EstimatedAttitudeAnglesCheckSaveId146 = 0; // 6 byte
	// set the EstimatedAngularRatesCheckSaveId147 parameter to false
	telemtry->EstimatedAngularRatesCheckSaveId147 = 0; // 6 byte
	// set the SatellitePositionECICheckSaveId148 parameter to false																																			 isn't that obvious already?
	telemtry->SatellitePositionECICheckSaveId148 = 0; // 6 byte
	// set the SatelliteVelocityECICheckSaveId149 parameter to false
	telemtry->SatelliteVelocityECICheckSaveId149 = 0; // 6 byte
	// set the SatellitePositionLLHCheckSaveId150 parameter to false
	telemtry->SatellitePositionLLHCheckSaveId150 = 0; // 6 byte
	// set the MagneticFieldVectorCheckSaveId151 parameter to 0
	telemtry->MagneticFieldVectorCheckSaveId151 = 0; // 6 byte
	// set the CoarseSunVectorCheckSaveId152 parameter to 0
	telemtry->CoarseSunVectorCheckSaveId152 = 0; // 6 byte
	// set the RateSensorRatesCheckSaveId155 parameter to 0
	telemtry->RateSensorRatesCheckSaveId155 = 0; // 6 byte
	// set the WheelSpeedCheckSaveId156 parameter to 0
	telemtry->WheelSpeedCheckSaveId156 = 0; // 6 byte
	// set the MagnetorquerCommandSaveId157 parameter to 0
	telemtry->MagnetorquerCommandSaveId157 = 0; // 6 byte
	// set the WheelSpeedCommandSaveId158 parameter to 0
	telemtry->WheelSpeedCommandSaveId158 = 0; // 6 byte
	// set the IGRFModelledMagneticFieldVectorCheckSaveId159 parameter to 0
	telemtry->IGRFModelledMagneticFieldVectorCheckSaveId159 = 0; // 6 byte
	// set the EstimatedGyroBiasCheckSaveId161 parameter to 0
	telemtry->EstimatedGyroBiasCheckSaveId161 = 0; // 6 byte
	// set the EstimationInnovationVectorCheckSaveId162 parameter to 0
	telemtry->EstimationInnovationVectorCheckSaveId162 = 0; // 6 byte
	// set the QuaternionErrorVectorCheckSaveId163 parameter to 0
	telemtry->QuaternionErrorVectorCheckSaveId163 = 0; // 6 byte
	// set the QuaternionCovarianceCheckSaveId164 parameter to 0
	telemtry->QuaternionCovarianceCheckSaveId164 = 0; // 6 byte
	// set the AngularRateCovarianceCheckSaveId165 parameter to 0
	telemtry->AngularRateCovarianceCheckSaveId165 = 0; // 6 byte
	// set the RawCSSCheckSaveId168 parameter to 0
	telemtry->RawCSSCheckSaveId168 = 0; // 6 byte
	// set the RawMagnetometerCheckSaveId170 parameter to 0
	telemtry->RawMagnetometerCheckSaveId170 = 0; // 6 byte
	// set the EstimatedQuaternionCheckSaveId218 parameter to 0
	telemtry->EstimatedQuaternionCheckSaveId218 = 0; // 6 byte
	// set the ECEFPositionCheckSaveId219 parameter to 0
	telemtry->ECEFPositionCheckSaveId219 = 0; // 6 byte

}
int GetDelay(stageTable ST)
{
	return ST -> StageData.delay;
}
 //! the telemtryStart function returns an initialized struct of the type ADCSPower
 void powerStart(ADCSPower * powerControl)
{
	 // set the signal_cubecontrol parameter to false
	powerControl->signal_cubecontrol = 0;
	// set the motor_cubecontrol parameter to false
	powerControl->motor_cubecontrol = 0;
	// set the pwr_cubewheel1 parameter to false
	powerControl->pwr_cubewheel = 0; // its cubewheel 1 or 2 or 3
	// set the pwr_motor parameter to false
	powerControl->pwr_motor = 0;
}
//! the createTable function returns an initialized struct of the type stageTable
void createTable(stageTable stageTableBuild)
{
	int i;
	for(i = 0;i < STAGE_TABLE_SIZE;i++)
	{
		stageTableBuild -> Raw[i] = 0;
	}

	// set the delay parmeter to 1000 [ms]
	stageTableBuild -> StageData.delay = START_DELAY;
	// set the control mode parameter to 0 no control
	stageTableBuild -> StageData.controlMode = START_CONTROL_MODE;
	// set the power parameter as seen in the powerStart
	powerStart(&(stageTableBuild -> StageData.powerControl));
	// set the estimation mode parameter to 0 no estimation
	stageTableBuild -> StageData.estimationMode = START_ESTIMATION_MODE;
	// set the telmetry parameter as seen in the telemtryStart function
	telemtryStart(&(stageTableBuild -> StageData.telemtry));
	// set the array
}
 /*! set the control mode
  * @param[controlModeST] stage table control mode
  */
unsigned char buildControlMode(unsigned char controlModeST)
 {
	unsigned char controlMode;
	 if(controlModeST == FAST_SPIN_DETUNBLING_STAGE_TABLE)
	 {
		 controlMode = FAST_SPIN_DETUNBLING_SAT;
	 }
	 else if(controlModeST == VERY_FAST_SPIN_DETUNBLING_STAGE_TABLE)
	 {
		 controlMode = VERY_FAST_SPIN_DETUNBLING_SAT;
	 }
	 else
	 {
		 controlMode = controlModeST;
	 }
	 return controlMode;
 }
//! update the stage table to the cube ADCS
 /*!
  * @param[stageTableGainData] contains the data that needs to be updated
  */

int Check_Table(stageTable stageTableGainData)
{
	cspace_adcs_powerdev_t get_ctrldev;
	cspaceADCS_getPwrCtrlDevice(0, &get_ctrldev);
	cspace_adcs_currstate_t adcs_state;
	int i;
	cspaceADCS_getCurrentState(0,&adcs_state );
	stageTableData Temp;
	for(i = 0; i < STAGE_TABLE_SIZE;i++)
	{
		Temp.Raw[i] = stageTableGainData -> Raw[i];
	}
	Temp.StageData.controlMode = adcs_state.fields.ctrl_mode;
	Temp.StageData.estimationMode = adcs_state.fields.attest_mode;
	Temp.StageData.powerControl.signal_cubecontrol = get_ctrldev.fields.signal_cubecontrol;
	Temp.StageData.powerControl.motor_cubecontrol = get_ctrldev.fields.motor_cubecontrol;
	Temp.StageData.powerControl.pwr_motor = get_ctrldev.fields.pwr_motor;
	for(i = 0; i < STAGE_TABLE_SIZE;i++)
	{
		if(Temp.Raw[i] == stageTableGainData -> Raw[i])
		{
			return -666;
		}
	}

	return 0;
}

int updateTable(stageTable stageTableGainData)
{
	int err = 0;
	// Initialize cspace_adcs_powerdev_t
	cspace_adcs_powerdev_t PowerADCS;
	// set the motor_cubecontrol parameter
	PowerADCS.fields.motor_cubecontrol = stageTableGainData -> StageData.powerControl.motor_cubecontrol;
	// set the pwr_cubesense parameter to FALSE
	PowerADCS.fields.pwr_cubesense = 0;
	// set the pwr_cubestar parameter to FALSE
	PowerADCS.fields.pwr_cubestar = 0;
	// set the pwr_cubewheel parameter
	PowerADCS.fields.pwr_cubewheel1 = stageTableGainData -> StageData.powerControl.pwr_cubewheel;
	// set the pwr_cubewheel2 parameter to FALSE
	PowerADCS.fields.pwr_cubewheel2 = 0;
	// set the pwr_cubewheel3 parameter to FALSE
	PowerADCS.fields.pwr_cubewheel3 = 0;
	// set the pwr_gps parameter to FALSE
	PowerADCS.fields.pwr_gps = 0;
	// set the pwr_motor parameter
	PowerADCS.fields.pwr_motor = stageTableGainData -> StageData.powerControl.pwr_motor;
	// set the signal_cubecontrol parameter
	PowerADCS.fields.signal_cubecontrol = stageTableGainData -> StageData.powerControl.signal_cubecontrol;
	// update the power in the cube ADCS

	err = cspaceADCS_setPwrCtrlDevice(ADCS_ID, &PowerADCS);
	if(err != 0 && err != -35)
	{
		return err;
	}
	vTaskDelay(2000);
	cspace_adcs_estmode_sel att_est =  stageTableGainData -> StageData.estimationMode;
		//set the estimation mode to cube ADCS
	err = cspaceADCS_setAttEstMode(ADCS_ID, att_est);
	if(err != 0 && err != -35)
	{
		return err;
	}

		vTaskDelay(2000);
	// Initialize the control mode parameter
	cspace_adcs_attctrl_mod_t control_Mode;
	// set the ctrl_mode parameter
	control_Mode.fields.ctrl_mode = buildControlMode(stageTableGainData -> StageData.controlMode);
	// set the override_flag
	control_Mode.fields.override_flag = 1;
	// set the timeout parameter
	control_Mode.fields.timeout = 0xfff;// set ADCS control mode
	err = cspaceADCS_setAttCtrlMode(ADCS_ID,&control_Mode);
	if(err != 0 && err != -35)
	{
		return err;
	}

	vTaskDelay(2000);
	// Initialize the estimation mode
	return Check_Table(stageTableGainData);

}
/*! a command that gets and translates the full stage table gotten from the ground and updates the stage table fully
 * @param[telemtry] contains the gotten stage table data
 * @param[stageTableGainData] the stage table to update
 */
int translateCommandFULL(unsigned char telemtry[], stageTable stageTableGainData)
{
	// a loop to update the stage table
	int i = 0;
	 for ( i = 0;  i < STAGE_TABLE_SIZE;i++)
	 {
		 //update a byte
		 stageTableGainData -> Raw[i] = telemtry[i];
	 }
	 // update the stage table to the cube ADCS
	return updateTable(stageTableGainData);
}
/*! a command that gets and translates the delay parameter stage table gotten from the ground and updates the stage table fully
 * @param[delay] contains the gotten stage table data
 * @param[stageTableGainData] the stage table to update
 */
 void translateCommandDelay( unsigned char delay[], stageTable stageTableGainData)
{
	 int i;
	 // a loop to update the stage table
	 for (i = 0;  i < SIZE_OF_DELAY; ++ i)
	 {
		 //update a byte
		 stageTableGainData -> Raw[i] = delay[i];
		 printf("%d\n",delay[i]);
	 }

}
 /*! a command that gets and translates the control Mode parameter stage table gotten from the ground and updates the stage table fully
  * @param[controlMode] contains the gotten stage table data
  * @param[stageTableGainData] the stage table to update
  */
 int translateCommandControlMode( unsigned char controlMode, stageTable stageTableGainData)
{
	 //update a byte
	stageTableGainData -> Raw[3] = controlMode;
	//update the cube ADCS with the new data
	return updateTable(stageTableGainData);
}
 /*! a command that gets and translates the power parameter stage table gotten from the ground and updates the stage table fully
   * @param[power] contains the gotten stage table data
   * @param[stageTableGainData] the stage table to update
   */
 int translateCommandPower(unsigned char power,stageTable stageTableGainData)
{
	 //update a byte
	stageTableGainData -> Raw[4] = power;
	//update the cube ADCS with the new data
	return updateTable(stageTableGainData);
}
 /*! a command that gets and translates the estimation Mode parameter stage table gotten from the ground and updates the stage table fully
    * @param[estimationMode] contains the gotten stage table data
    * @param[stageTableGainData] the stage table to update
    */
 int  translateCommandEstimationMode( unsigned char estimationMode,stageTable stageTableGainData)
{
	 //update a byte
	 stageTableGainData -> Raw[5] = estimationMode;
	 //update the cube ADCS with the new data
	 return updateTable(stageTableGainData);
}
 /*! a command that gets and translates the telemtry parameter stage table gotten from the ground and updates the stage table fully
     * @param[telemtry] contains the gotten stage table data
     * @param[stageTableGainData] the stage table to update
     */
 void translateCommandTelemtry( unsigned char telemtry[],stageTable stageTableGainData)
{
	 int i;
	 // a loop to update the telmtry
	 for (i = 0;  i < 3; ++ i)
	 	 {
		 //update a byte
	 		 stageTableGainData -> Raw[i+6] = telemtry[i];
	 	 }
}
/*! a function that return an array of the ADCS Stage Table according to the given data
 * @param[byteData] decided what parts of the stage table to return
 * @param[stageTableUpdateData] pointer to the table that will return the data
 */
 void getTableTo(stageTable stageTableUpdateData,unsigned char* Data)
{
	 int i;
	 for(i = 0; i  < STAGE_TABLE_SIZE;i++)
	 {
		 Data[i] = stageTableUpdateData -> Raw[i];
	 }
}
/*! return parameter EstimatedAttitudeAnglesCheckSaveId146
 * @param[stagetable] the stage table that contains the data
 */
 int checkEstimatedAttitudeAngles(stageTable stagetable)
{
	return stagetable->StageData.telemtry.EstimatedAttitudeAnglesCheckSaveId146;
}
 /*! return parameter EstimatedAngularRatesCheckSaveId147
  * @param[stagetable] the stage table that contains the data
  */
 int checkEstimatedAngularRates(stageTable stageTable)
 {
 	return stageTable->StageData.telemtry.EstimatedAngularRatesCheckSaveId147;
 }
 /*! return parameter SatellitePositionECICheckSaveId148
  * @param[stagetable] the stage table that contains the data
  */
int checkSatellitePositionECI(stageTable stageTable)
{
	return stageTable->StageData.telemtry.SatellitePositionECICheckSaveId148;
}
/*! return parameter SatelliteVelocityECICheckSaveId149
 * @param[stagetable] the stage table that contains the data
 */
 int checkSatelliteVelocityECI(stageTable stageTable)
{
	return stageTable->StageData.telemtry.SatelliteVelocityECICheckSaveId149;
}
 /*! return parameter SatellitePositionLLHCheckSaveId150
  * @param[stagetable] the stage table that contains the data
  */

 int checkSatellitePositionLLH(stageTable stageTable)
{
	return stageTable->StageData.telemtry.SatellitePositionLLHCheckSaveId150;
}
 /*! return parameter MagneticFieldVectorCheckSaveId151
  * @param[stagetable] the stage table that contains the data
  */

 int checkMagneticFieldVector(stageTable stageTable)
{
	return stageTable->StageData.telemtry.MagneticFieldVectorCheckSaveId151;
}
 /*! return parameter CoarseSunVectorCheckSaveId152
  * @param[stagetable] the stage table that contains the data
  */
 int checkCoarseSunVector(stageTable stageTable)
{
	return stageTable->StageData.telemtry.CoarseSunVectorCheckSaveId152;
}
 /*! return parameter RateSensorRatesCheckSaveId155
  * @param[stagetable] the stage table that contains the data
  */
 int checkRateSensorRates(stageTable stageTable)
{
	return stageTable->StageData.telemtry.RateSensorRatesCheckSaveId155;
}
 /*! return parameter WheelSpeedCheckSaveId156
  * @param[stagetable] the stage table that contains the data
  */
 int checkWheelSpeedcheck(stageTable stageTable)
{
	return stageTable->StageData.telemtry.WheelSpeedCheckSaveId156;
}
 /*! return parameter MagnetorquerCommandSaveId157
  * @param[stagetable] the stage table that contains the data
  */
int checkMagnetorquercmd(stageTable stageTable)
{
	return stageTable->StageData.telemtry.MagnetorquerCommandSaveId157;
}
/*! return parameter WheelSpeedCommandSaveId158
 * @param[stagetable] the stage table that contains the data
 */
int checkWheelSpeedcmd(stageTable stageTable)
{
	return stageTable ->StageData.telemtry.WheelSpeedCommandSaveId158;
}
/*! return parameter IGRFModelledMagneticFieldVectorCheckSaveId159
 * @param[stagetable] the stage table that contains the data
 */
 int checkIGRFModelledMagneticFieldVector(stageTable stageTable)
{
	return stageTable->StageData.telemtry.IGRFModelledMagneticFieldVectorCheckSaveId159;
}
 /*! return parameter EstimatedGyroBiasCheckSaveId161
  * @param[stagetable] the stage table that contains the data
  */
 int checkEstimatedGyroBiascheck(stageTable stageTable)
{
	return stageTable->StageData.telemtry.EstimatedGyroBiasCheckSaveId161;
}
 /*! return parameter EstimationInnovationVectorCheckSaveId162
  * @param[stagetable] the stage table that contains the data
  */
 int checkEstimationInnovationVector(stageTable stageTable)
{
	return stageTable->StageData.telemtry.EstimationInnovationVectorCheckSaveId162;
}
 /*! return parameter QuaternionErrorVectorCheckSaveId163
  * @param[stagetable] the stage table that contains the data
  */
 int checkQuaternionErrorVector(stageTable stageTable)
{
	return stageTable->StageData.telemtry.QuaternionErrorVectorCheckSaveId163;
}
 /*! return parameter QuaternionCovarianceCheckSaveId164
  * @param[stagetable] the stage table that contains the data
  */
 int checkQuaternionCovariance(stageTable stageTable)
{
	return stageTable->StageData.telemtry.QuaternionCovarianceCheckSaveId164;
}
 /*! return parameter AngularRateCovarianceCheckSaveId165
  * @param[stagetable] the stage table that contains the data
  */
 int checkAngularRateCovariance(stageTable stageTable)
{
	return stageTable->StageData.telemtry.AngularRateCovarianceCheckSaveId165;
}
 /*! return parameter RawCSSCheckSaveId168
  * @param[stagetable] the stage table that contains the data
  */
 int checkRawCSS(stageTable stageTable)
{
	return stageTable->StageData.telemtry.RawCSSCheckSaveId168;
}
 /*! return parameter RawMagnetometerCheckSaveId170
  * @param[stagetable] the stage table that contains the data
  */
 int checkRawMagnetometer(stageTable stageTable)
{
	return stageTable->StageData.telemtry.RawMagnetometerCheckSaveId170;
}
 /*! return parameter EstimatedQuaternionCheckSaveId218
 * @param[stagetable] the stage table that contains the data
 */
 int checkEstimatedQuaternion(stageTable stageTable)
{
	return stageTable->StageData.telemtry.EstimatedQuaternionCheckSaveId218;
}
 /*! return parameter ECEFPositionCheckSaveId219
  * @param[stagetable] the stage table that contains the data
  */
 int checkECEFPosition(stageTable stageTable)
{
	return stageTable->StageData.telemtry.ECEFPositionCheckSaveId219;
}

int checkSunModelle(stageTable stageTable)
{
	return stageTable->StageData.telemtry.Sun_Modelle;

}
