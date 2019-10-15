/*
 * ADCS.h
 *
 *  Created on: 3 May 2018
 *      Author: Michael
 */

#ifndef ADCS_H_
#define ADCS_H_
#include <stdio.h>
#include <stdlib.h>
#include <satellite-subsystems/cspaceADCS.h>
#include <satellite-subsystems/cspaceADCS_types.h>
#include <hal/Storage/FRAM.h>

#include "ADCS/Stage_Table.h"
#include "Global/TLM_management.h"
#include "Global/Global.h"

#define START_ESTIMATION_MODE 0
//! define Test as FALSE == 0
#define TEST FALSE
//! size of telemetry in the cube adcs = 6
#define SIZE_OF_TELEMTRY 6
//! the file name of the CSS
#define NAME_OF_CSS_DATA_FILE ("Css_data")
//! the file name of the magnetic field
#define NAME_OF_MAGNETIC_FIELD_DATA_FILE ("Magnetic_Feild")
//! the file name of the css sun vector
#define CSS_SUN_VECTOR_DATA_FILE ("CSS_Sun_vector")
//! the file name of the wheel speed
#define WHEEL_SPEED_DATA_FILE ("Wheel_Speed")
//! the file name of the sensor rate
#define	SENSOR_RATE_DATA_FILE ("Sensor_Rate")
//! the file name of the magnetorquer cmd
#define MAGNETORQUER_CMD_FILE ("MAG_CMD")
//! the file name of the wheel speed cmd
#define WHEEL_SPEED_CMD_FILE ("Whell_CMD")
//! the file name of the taw magnetometer
#define RAW_MAGNETOMETER_FILE ("MAG_Raw")
//! the file name of the UGRF model
#define IGRF_MODEL_MAGNETIC_FIELD_VECTOR_FILE ("IGRF_MODEL")
//! the file name of the Gyro bias
#define GYRO_BIAS_FILE ("Gyro_BIAS")
//! the file name of the innovation vector
#define INNOVATION_VECTOR_FILE ("Inno_Vextor")
//! the file name of the error vector
#define ERROR_VECTOR_FILE ("Error_Vec")
//! the file name of the quaternion covariance
#define QUATERNION_COVARIANCE_FILE ("QUATERNION_COVARIANCE")
//! the file name of the angular rate covariance
#define ANGULAR_RATE_COVARIANCE_FILE ("ANGULAR_RATE_COVARIANCE")
//! the file name of the estimated angles
#define ESTIMATED_ANGLES_FILE ("ESTIMATED_ANGLES")
//! the file name of the angular rates
#define ESTIMATED_ANGULAR_RATE_FILE ("Estimated_AR")
//! the file name of the eci postion
#define NAME_OF_ECI_POSITION_DATA_FILE ("ECI_POS")
//! the file name of the satallite velocity
#define NAME_OF_SATALLITE_VELOCITY_DATA_FILE ("SAV_Vel")
//! the file name of the ecef position
#define NAME_OF_ECEF_POSITION_DATA_FILE ("ECEF_POS")
//! the file name of the LLH postion
#define NAME_OF_LLH_POSTION_DATA_FILE ("LLH_POS")
//! the file name of the estimated quaternion
#define ESTIMATED_QUATERNION_FILE ("EST_QUATERNION")

#define SUN_MODELLE_FILE ("Sun_Mode")
//! location of pid bias param
#define PID_BIAS_X_LOCATION 0
//! location of pid P param
#define PID_K_P_X_LOCATION 0
//! location of pid I param
#define PID_K_I_X_LOCATION 0
//! location of pid D param
#define PID_K_D_X_LOCATION 0
//! location of pid bias param
#define PID_BIAS_Z_LOCATION 0
//! location of pid P param
#define PID_K_P_Z_LOCATION 0
//! location of pid I param
#define PID_K_I_Z_LOCATION 0
//! location of pid D param
#define PID_K_D_Z_LOCATION 0
//! adcs id value

//! ascs i2c adddress
#define ADCS_I2C_ADRR 0x57
//! gain a full stage table to the
#define FULLSTAGETABLE  0b11111111
#define X_AXIS 0
#define Z_AXIS 1
#define GET_X_AXIS ((data[0] << 8) + data[1])
#define GET_Z_AXIS ((data[4] << 8) + data[5])

#define ADCS_ID 0

typedef enum en_t
{
	aasdf,bfdsasdf,cdsafdsa

}EN;

//! a function the starts the loop called only one time
void init_adcs(Boolean activation);
#endif /* ADCS_H_ */
