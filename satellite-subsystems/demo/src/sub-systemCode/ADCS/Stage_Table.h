/*
 * srage_Table.h
 *
 *  Created on: 14 Apr 2018
 *      Author: Michael
 */

#ifndef STAGE_TABLE_H_
#define STAGE_TABLE_H_

//! The structure TLM_Data_t contains the Delay in the Stage Table and the telemtry part of the Stage table
/*! this structere contains the delay in the stage table and the telmtry part of the stage table as shown in the flowing link:
 * https://drive.google.com/open?id=1vyo0ZM_S_6jSiOzThDQ1VauMdaNUAnhaFKsNeQK_CA0
 * the flowing paramaters contain what telemtry is being saved in the loop
*/
typedef struct
{
	//! a parameter that holds if the satellite saves the Estimated Attitude Angles Id = 146
	char EstimatedAttitudeAnglesCheckSaveId146 : 1,
	//! a parameter that holds if the satellite saves the Estimated Angular Rates Id = 147
	EstimatedAngularRatesCheckSaveId147 : 1,
	//! a parameter that holds if the satellite saves the Satellite Position ECI Id = 148
	SatellitePositionECICheckSaveId148 : 1,
	//! a parameter that holds if the satellite saves the Satellite Velocity ECI Id = 149
	SatelliteVelocityECICheckSaveId149 : 1,
	//! a parameter that holds if the satellite saves the Satellite Position LLH Id = 150
	SatellitePositionLLHCheckSaveId150 : 1,
	//! a parameter that holds if the satellite saves the Magnetic Field Vector Id = 151
	MagneticFieldVectorCheckSaveId151 : 1,
	//! a parameter that holds if the satellite saves the Coarse Sun Vector Id = 152
	CoarseSunVectorCheckSaveId152 : 1,
	//! a parameter that holds if the satellite saves the Rate Sensor Rates Id = 155
	RateSensorRatesCheckSaveId155 : 1;
	//! a parameter that holds if the satellite saves the Wheel Speed Id = 156
	char WheelSpeedCheckSaveId156 : 1,
	//! a parameter that holds if the satellite saves the Magnetorquer Command  ID = 157
	MagnetorquerCommandSaveId157 : 1,
	//! a parameter that holds if the satellite saves the Wheel Speed Command  ID = 158
	WheelSpeedCommandSaveId158 : 1,
	//! a parameter that holds if the satellite saves the IGRF Modelled Magnetic Field Vector Id = 159
	IGRFModelledMagneticFieldVectorCheckSaveId159 : 1,
	//! a parameter that holds if the satellite saves the Estimated Gyro Bias Id = 161
	EstimatedGyroBiasCheckSaveId161 : 1,
	//! a parameter that holds if the satellite saves the Estimation Innovation Vector Id = 162
	EstimationInnovationVectorCheckSaveId162 : 1,
	//! a parameter that holds if the satellite saves the Quaternion Error Vector Id = 163
	QuaternionErrorVectorCheckSaveId163 : 1,
	//! a parameter that holds if the satellite saves the Quaternion Covariance Id = 164
	QuaternionCovarianceCheckSaveId164 : 1,
	//! a parameter that holds if the satellite saves the Angular Rate Covariance Id = 165
	AngularRateCovarianceCheckSaveId165 : 1;
	//! a parameter that holds if the satellite saves the Raw CSS Id = 168
	char	RawCSSCheckSaveId168 : 1,
	//! a parameter that holds if the satellite saves the Raw Magnetometer Id = 170
	RawMagnetometerCheckSaveId170 : 1,
	//! a parameter that holds if the satellite saves the Estimated Quaternion Id = 218
	EstimatedQuaternionCheckSaveId218 : 1,
	//! a parameter that holds if the satellite saves the ECEF Position Id = 219
	ECEFPositionCheckSaveId219 : 1,
	Sun_Modelle : 1;
}TLMData;


//! a pointer to a struct that contains all the information about the stage table
typedef union stageTableData* stageTable;
//! a function that creates a stage Table with all the parameters in the starting position down 00000000 00000011 11101000 00000000 00000000 00000000 00000000 00000000 00000000
void createTable(stageTable stageTableBuild);
/*! updates the stage table data to the cubeADCS system
 * @param[stageTableGainData] contains the stage table data
 */
int Check_Table(stageTable stageTableGainData);
int updateTable(stageTable stageTableGainData);
/*! Get a full stage table from the ground and update the full stage table
 * @param[telemtry] contains the new stage table data
 * @param[stagetable] the stage table that will be updated
 */
int translateCommandFULL(unsigned char telemtry[], stageTable stagetable);
/*! Get the delay parameter stage table from the ground and update it to the stage table
 * @param[delay] contains the new delay
 * @param[stagetable] the stage table that will be updated
 */
void translateCommandDelay(unsigned char delay[3], stageTable stagetable);
/*! Get the control mode parameter stage table from the ground and update it to the stage table
 * @param[controlMode] contains the new control Mode
 * @param[stagetable] the stage table that will be updated
 */
int translateCommandControlMode(unsigned char controlMode, stageTable stagetable);
/*! Get the Power parameter stage table from the ground and update it to the stage table
 * @param[power] contains the new power
 * @param[stagetable] the stage table that will be updated
 */
int translateCommandPower( unsigned char power,stageTable stageTableGainData);
/*! Get the Estimation mode parameter stage table from the ground and update it to the stage table
 * @param[estimationMode] contains the new estimation Mode
 * @param[stagetable] the stage table that will be updated
 */
int translateCommandEstimationMode(unsigned char estimationMode,stageTable stagetable);
/*! Get the Telemtry parameter stage table from the ground and update it to the stage table
 *  @param[telemtry] contains the new telemtry
 * @param[stagetable] the stage table that will be updated
 *
 */
void translateCommandTelemtry(unsigned char telemtry[3],stageTable stagetable);
/*! Get the current stage table according to the data char
 * @param[Data] caontain what parametrs of the stage table will be returned
 * @param[stagetable] the stage table to be returned
 */
void getTableTo(stageTable stagetable,unsigned char* Data);
/*!a function that checks if the system need to save the 146 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkEstimatedAttitudeAngles(stageTable stagetable);
/*!a function that checks if the system need to save the 147 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkSatellitePositionECI(stageTable stagetable);
/*!a function that checks if the system need to save the 148 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkEstimatedAngularRates(stageTable stagetable);
/*!a function that checks if the system need to save the 149 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkSatelliteVelocityECI(stageTable stagetable);
/*!a function that checks if the system need to save the 150 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkSatellitePositionLLH(stageTable Stagetable);
/*!a function that checks if the system need to save the 151 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkMagneticFieldVector(stageTable Stagetable);
/*!a function that checks if the system need to save the 152 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkCoarseSunVector(stageTable Stagetable);
/*!a function that checks if the system need to save the 155 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkRateSensorRates(stageTable Stagetable);
/*!a function that checks if the system need to save the 156 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkWheelSpeedcheck(stageTable Stagetable);
/*!a function that checks if the system need to save the 157 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkMagnetorquercmd(stageTable stageTable);
/*!a function that checks if the system need to save the 158 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkWheelSpeedcmd(stageTable stageTable);
/*!a function that checks if the system need to save the 159 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkIGRFModelledMagneticFieldVector(stageTable Stagetable);
/*!a function that checks if the system need to save the 161 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkEstimatedGyroBiascheck(stageTable Stagetable);
/*!a function that checks if the system need to save the 162 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkEstimationInnovationVector(stageTable Stagetable);
/*!a function that checks if the system need to save the 163 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkQuaternionErrorVector(stageTable Stagetable);
/*!a function that checks if the system need to save the 164 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkQuaternionCovariance(stageTable Stagetable);
/*!a function that checks if the system need to save the 165 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkAngularRateCovariance(stageTable Stagetable);
/*!a function that checks if the system need to save the 168 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkRawCSS(stageTable Stagetable);
/*!a function that checks if the system need to save the 170 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkRawMagnetometer(stageTable Stagetable);
/*!a function that checks if the system need to save the 218 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkEstimatedQuaternion(stageTable Stagetable);
/*!a function that checks if the system need to save the 219 id telemtry
 * @param[stagetable] the stage table which the data is taken from
 */
int checkECEFPosition(stageTable Stagetable);
int checkSunModelle(stageTable stageTable);
#endif /* STAGETABLEH */
