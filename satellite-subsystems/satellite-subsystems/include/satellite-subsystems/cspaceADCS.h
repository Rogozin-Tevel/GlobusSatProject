/*
 * eslADCS.h
 *
 *  Created on: 2 dec. 2013
 *      Author: malv
 */

#ifndef ESLADCS_H_
#define ESLADCS_H_

#include "cspaceADCS_types.h"
#include <hal/boolean.h>


/**
 *  @brief      Initialize the cspaceADCS with the corresponding i2cAddress.
 *  @note       This function can only be called once.
 *  @param[in]  address array of cspaceADCS I2C Addresses.
 *  @param[in]  number number of attached cspaceADCS in the system to be initialized.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_initialize(unsigned char *address, unsigned char number);

/**** Telecommand ****/

/**
 *  @brief      Reset the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_componentReset(unsigned char index);

/**
 *  @brief      Set the current time of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  unix_time time struct to be set.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setCurrentTime(unsigned char index, cspace_adcs_unixtm_t* unix_time);

/**
 *  @brief      Set the cache state in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  current_state state to be defined.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_cacheStateADCS(unsigned char index, unsigned char current_state);

/**
 *  @brief      Set the current run mode on the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  runmode current mode to be set.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setRunMode(unsigned char index, cspace_adcs_runmode_t runmode);

/**
 *  @brief      Set a device in the cspaceADCS unit on.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  device_ctrl the device to be controlled.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setPwrCtrlDevice(unsigned char index, cspace_adcs_powerdev_t* device_ctrl);

/**
 * @brief		Clear error flags of the cspaceADCS unit
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  clear control which flags will be cleared
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_clearErrors(unsigned char index, cspace_adcs_clearflags_t clear);

/**
 *  @brief      Set the magnetorquer output in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  cmd_pulse command pulse to set as an output.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setMagOutput(unsigned char index, cspace_adcs_magnetorq_t* cmd_pulse);

/**
 *  @brief      Set new wheel speed value in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  cmd_speed struct of wheel speed components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setWheelSpeed(unsigned char index, cspace_adcs_wspeed_t* cmd_speed);

/**
 *  @brief      Deploy magnetometer boom in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  timeout deployment actuation timeout value.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_deployMagBoomADCS(unsigned char index, unsigned char timeout);

/**
 *  @brief      Set attitude control mode in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  att_ctrl attitude control parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setAttCtrlMode(unsigned char index, cspace_adcs_attctrl_mod_t* att_ctrl);

/**
 *  @brief      Set attitude estimation mode in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  att_est attitude estimation parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setAttEstMode(unsigned char index, cspace_adcs_estmode_sel att_est);

/**
 *  @brief      Set cam1 sensor configuration in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  config_data cam1 configuration parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setCam1SensorConfig(unsigned char index, cspace_adcs_camsencfg_t* config_data);

/**
 *  @brief      Set cam2 sensor configuration in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  config_data cam2 configuration parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setCam2SensorConfig(unsigned char index, cspace_adcs_camsencfg_t* config_data);

/**
 *  @brief      Set magnetometer mounting configuration in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  config_data magnetometer mounting configuration parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setMagMountConfig(unsigned char index, cspace_adcs_magmountcfg_t* config_data);

/**
 *  @brief      Set magnetometer offset and scaling configuration in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  config_data magnetometer offset and scaling configuration parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setMagOffsetScalingConfig(unsigned char index, cspace_adcs_offscal_cfg_t* config_data);

/**
 *  @brief      Set magnetometer sensitivity configuration in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  config_data magnetometer sensitivity configuration parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setMagSensConfig(unsigned char index, cspace_adcs_magsencfg_t* config_data);

/**
 *  @brief      Set rate sensor configuration in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  config_data rate sensor configuration parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setRateSensorConfig(unsigned char index, cspace_adcs_ratesencfg_t* config_data);

/**
 *  @brief      Set star tracker configuration in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  config_data star tracker configuration parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setStarTrackerConfig(unsigned char index, cspace_adcs_startrkcfg_t* config_data);

/**
 *  @brief      Set estimation parameters 1 in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  est_param estimation parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setEstimationParam1(unsigned char index, cspace_adcs_estparam1_t* est_param);

/**
 *  @brief      Set estimation parameters 2 in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  est_param estimation parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setEstimationParam2(unsigned char index, cspace_adcs_estparam2_t* est_param);

/**
 * @brief		Set the mode of operation for the magnetometer
 * @param[in]	index index of the cspaceADCS unit.
 * @param[in]	magmode Mode of operations for the magnetometer
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setMagnetometerMode(unsigned char index, cspace_adcs_magmode_t magmode);

/**
 * @brief		Set SGP4 orbit estimation parameters
 * @param[in]	index index of the cpsaceADCS unit
 * @param[in]	parameters Orbital parameters to be used, usually derived from TLE.
 * The entries in the array are:
 * 0 - Inclination (degrees)
 * 1 - Eccentricity
 * 2 - Right-ascension of the ascending node (degrees)
 * 3 - Argument of perigee (degrees)
 * 4 - B-star drag term
 * 5 - Mean motion (orbits per day)
 * 6 - Mean anomaly (degrees)
 * 7 - Epoch (year.day)
 * @return		error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_setSGP4OrbitParameters(unsigned char index, double* parameters);

/**
 *  @brief      Save configuration parameters in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_saveConfig(unsigned char index);

/**
 *  @brief      Save orbit parameters in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_saveOrbitParam(unsigned char index);

/**
 *  @brief      Save an image in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  savimag_param save image parameters struct.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_saveImage(unsigned char index, cspace_adcs_savimag_t savimag_param);

/**
 *  @brief      Set the boot index of the program to be used by the bootloader.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  bootindex index of the program to be booted.
 *  @return     error status of the function, defined in "SSerrors.h".
 *  @note		This command only works when the ADCS bootloader is active
 */
int cspaceADCS_BLSetBootIndex(unsigned char index, cspace_adcs_bootprogram bootindex);

/**
 *  @brief      Tell the boot loader to run the selected program
 *  @param[in]  index index of the cspaceADCS unit.
 *  @return     error status of the function, defined in "SSerrors.h".
 *  @note		This command only works when the ADCS bootloader is active
 */
int cspaceADCS_BLRunSelectedProgram(unsigned char index);

/**** Telemetry ****/

/**
 *  @brief      Get general status information in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  info_data struct of information data components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getGeneralInfo(unsigned char index, cspace_adcs_geninfo_t* info_data);

/**
 *  @brief      Get boot and program info of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  boot_info struct with boot and program info.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getBootProgramInfo(unsigned char index, cspace_adcs_bootinfo_t* boot_info);

/**
 *  @brief      Get SRAM latchup counters of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  sramcount struct with SRAM latchup counters.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getSRAMLatchupCounters(unsigned char index, cspace_adcs_sramlatchupcnt_t* sramcount);

/**
 *  @brief      Get EDAC counters of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  edaccount struct with EDAC counters.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getEDACCounters(unsigned char index, cspace_adcs_edaccnt_t* edaccount);


/**
 *  @brief      Get the current time of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] unix_time struct of unix time components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getCurrentTime(unsigned char index, cspace_adcs_unixtm_t* unix_time);

/**
 *  @brief      Get communication status information in the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[in]  comm_status struct of communication status components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getCommStatus(unsigned char index, cspace_adcs_commstat_t* comm_status);

/**
 * @brief		Get the current state of the cspaceADCS unit.
 * @param[in]	index index of the cspaceADCS unit.
 * @param[out]	current_state Struct for storing the current state.
 * @return		error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getCurrentState(unsigned char index, cspace_adcs_currstate_t* current_state);

/**
 *  @brief      Get the magnetic field vector of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] mag_field struct of magnetic field vector.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getMagneticFieldVec(unsigned char index, cspace_adcs_magfieldvec_t* mag_field);

/**
 *  @brief      Get the coarse sun vector of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] coarse_sun struct of coarse sun vector.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getCoarseSunVec(unsigned char index, cspace_adcs_sunvec_t* coarse_sun);

/**
 *  @brief      Get the fine sun vector of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] fine_sun struct of fine sun vector.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getFineSunVec(unsigned char index, cspace_adcs_sunvec_t* fine_sun);

/**
 *  @brief      Get the nadir vector of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] nadir_vec struct of nadir vector.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getNadirVector(unsigned char index, cspace_adcs_nadirvec_t* nadir_vec);

/**
 *  @brief      Get the rate sensor measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] sen_rates struct of rate sensor measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getSensorRates(unsigned char index, cspace_adcs_angrate_t* sen_rates);

/**
 *  @brief      Get the wheel speed measurement of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] wheel_speed struct of wheel speed measurement.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getWheelSpeed(unsigned char index, cspace_adcs_wspeed_t* wheel_speed);

/**
 * @brief		Get the magnetorquer commands.
 * @param[in]	index index of the cspaceADCS unit.
 * @param[out]	mag_cmd struct with the magnetorquer commands
 * @return		error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getMagnetorquerCmd(unsigned char index, cspace_adcs_magtorqcmd_t* mag_cmd);

/**
 * @brief		Get the wheel speed commands.
 * @param[in]	index index of the cspaceADCS unit.
 * @param[out]	wheelspeed_cmd struct with the wheel speed commands
 * @return		error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getWheelSpeedCmd(unsigned char index, cspace_adcs_wspeed_t* wheelspeed_cmd);

/**
 *  @brief      Get the raw cam2 sensor capture and detection results of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_cam2 struct of cam2 sensor capture and detection results.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawCam2Sensor(unsigned char index, cspace_adcs_rawcam_t* raw_cam2);

/**
 *  @brief      Get the raw cam1 sensor capture and detection results of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_cam1 struct of cam1 sensor capture and detection results.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawCam1Sensor(unsigned char index, cspace_adcs_rawcam_t* raw_cam1);

/**
 *  @brief      Get the CSS from 1 to 6 of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_css struct of raw css from CSS 1 to 6.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawCss1_6Measurements(unsigned char index, cspace_adcs_rawcss1_6_t* raw_css);

/**
 *  @brief      Get the CSS from 7 to 10 of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_css struct of raw css from CSS 7 to 10.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawCss7_10Measurements(unsigned char index, cspace_adcs_rawcss7_10_t* raw_css);

/**
 *  @brief      Get the raw magnetometer measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_nadir struct of raw magnetometer measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawMagnetometerMeas(unsigned char index, cspace_adcs_rawmagmeter_t* raw_mag);

/**
 *  @brief      Get the CubeSense current measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] curr_meas struct of CubeSense current measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getCSenseCurrentMeasurements(unsigned char index, cspace_adcs_csencurrms_t* curr_meas);

/**
 *  @brief      Get the CubeControl current measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] curr_meas struct of CubeControl current measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getCControlCurrentMeasurements(unsigned char index, cspace_adcs_cctrlcurrms_t* curr_meas);

/**
 *  @brief      Get the wheel telemetry currents of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] wheelcurrents struct containing the wheel telemetry currents.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getWheelCurrentsTlm(unsigned char index, cspace_adcs_wheelcurr_t* wheelcurrents);

/**
 *  @brief      Get the ADCS telemetry temperatures of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] temp_tlm struct containing the adcs telemetry temperatures.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getADCSTemperatureTlm(unsigned char index, cspace_adcs_msctemp_t* temp_tlm);

/**
 *  @brief      Get the rate sensor telemetry temperatures of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] temp_tlm struct containing the rate sensor telemetry temperatures.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRateSensorTempTlm(unsigned char index, cspace_adcs_ratesen_temp_t* temp_tlm);

/**
 *  @brief      Get the raw GPS status of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_nadir struct of raw GPS status variables.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawGpsStatus(unsigned char index, cspace_adcs_rawgpssta_t* raw_gps_status);

/**
 *  @brief      Get the raw GPS time of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_nadir struct of raw GPS time variables.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawGpsTime(unsigned char index, cspace_adcs_rawgpstm_t* raw_gps_time);

/**
 *  @brief      Get the raw GPS x position and velocity component of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_nadir struct of raw GPS x position and velocity components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawGpsX(unsigned char index, cspace_adcs_rawgps_t* raw_gps_x);

/**
 *  @brief      Get the raw GPS y position and velocity component of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_nadir struct of raw GPS y position and velocity components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawGpsY(unsigned char index, cspace_adcs_rawgps_t* raw_gps_y);

/**
 *  @brief      Get the raw GPS z position and velocity component of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_nadir struct of raw GPS z position and velocity components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawGpsZ(unsigned char index, cspace_adcs_rawgps_t* raw_gps_z);

/**
 *  @brief      Get the ADCS general state telemetry of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] adcs_state struct containing the adcs general state telemetry.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getStateTlm(unsigned char index, cspace_adcs_statetlm_t* adcs_state);

/**
 * @brief		Get the general sensor measurements of the cspaceADCS unit.
 * @param[in]	index index of the cspaceADCS unit.
 * @param[out]	measure_adc adcs_measurements_t struct components.
 * @return		error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getADCSMeasurements(unsigned char index, cspace_adcs_measure_t* measure_adc);

/**
 *  @brief      Get the actuators commands of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] actuators_cmds struct of actuators_cmds components.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getActuatorsCmds(unsigned char index, cspace_adcs_actcmds_t* actuators_cmds);

/**
 * @brief		Get the general estimation metadata of the cspaceADCS unit.
 * @param[in]	index index of the cspaceADCS unit.
 * @param[out]	metadata cspaceADCSEstimationMetadata struct components.
 * @return		error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getEstimationMetadata(unsigned char index, cspace_adcs_estmetadata_t* metadata);

/**
 *  @brief      Get the raw sensor measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] sensor_meas struct of raw sensor measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getRawSensorMeasurements(unsigned char index, cspace_adcs_rawsenms_t* sensor_meas);

/**
 *  @brief      Get the power and temperature telemetry measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] sensor_meas struct containig the power and temperature telemetry measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getPowTempMeasTLM(unsigned char index, cspace_adcs_pwtempms_t* sensor_meas);

/**
 *  @brief      Get the adcs execution times of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] exc_times struct containig the adcs execution times.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getADCSExcTimes(unsigned char index, cspace_adcs_exctm_t* exc_times);

/**
 *  @brief      Get the power control device information of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] pwr_device struct of power device selection.
 *  @return     error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getPwrCtrlDevice(unsigned char index, cspace_adcs_powerdev_t* pwr_device);

/**
 *  @brief      Get the adcs miscellaneous currents telemetry of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] misc_curr struct containing the miscellaneous telemetry currents.
 *  @return     error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getMiscCurrentMeas(unsigned char index, cspace_adcs_misccurr_t* misc_curr);

/**
 *  @brief      Get the commanded attitude angles of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] cmdangles struct containing the commanded angles.
 *  @return     error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getCommandedAttitudeAngles(unsigned char index, cspace_adcs_cmdangles_t* cmdangles);

/**
 *  @brief      Get the adcs raw gps measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] raw_gps struct containing the raw gps measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getADCSRawGPSMeas(unsigned char index, cspace_adcs_rawgpsms_t* raw_gps);

/**
 * @brief		Get the startracker telemetry of the cspaceADCS unit.
 * @param[in]	index index of the cspaceADCS unit.
 * @param[out]	startrack_tlm struct containing the startracker telemetry
 * @return     error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getStartrackerTLM(unsigned char index, cspace_adcs_startrackertlm_t* startrack_tlm);

/**
 *  @brief      Get the adcs raw gps measurements of the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] redund_mag struct containing the raw redundant magnetometer measurements.
 *  @return     error status of the initialization, defined in "SSerrors.h"
 */
int cspaceADCS_getADCRawRedMag(unsigned char index, cspace_adcs_rawmagmeter_t* redund_mag);

/**
 *  @brief      Get ACP execution state from the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] acploop_cmds ACP execution state.
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getACPExecutionState(unsigned char index, cspace_adcs_acp_info_t* acploop_cmds);

/**
 *  @brief      Get current configuration from the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] config Configuration parameters
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getADCSConfiguration(unsigned char index, unsigned char* config);

/**
 *  @brief      Get current SGP4 orbit parameters from the cspaceADCS unit.
 *  @param[in]  index index of the cspaceADCS unit.
 *  @param[out] parameters Orbital parameters currently in use,
 * The entries in the array are:
 * 0 - Inclination (degrees)
 * 1 - Eccentricity
 * 2 - Right-ascension of the ascending node (degrees)
 * 3 - Argument of perigee (degrees)
 * 4 - B-star drag term
 * 5 - Mean motion (orbits per day)
 * 6 - Mean anomaly (degrees)
 * 7 - Epoch (year.day)
 *  @return     error status of the initialization, defined in "SSerrors.h".
 */
int cspaceADCS_getSGP4OrbitParameters(unsigned char index, double* parameters);

#endif /* ESLADCS_H_ */
