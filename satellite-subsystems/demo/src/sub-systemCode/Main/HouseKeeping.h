/*
 * HouseKeeping.h
 *
 *  Created on: Jan 18, 2019
 *      Author: elain
 */

#ifndef HOUSEKEEPING_H_
#define HOUSEKEEPING_H_

#include "../Global/Global.h"
#include "../COMM/GSC.h"
#include "../Global/TLM_management.h"

#include <satellite-subsystems/cspaceADCS_types.h>

#define TASK_HK_HIGH_RATE_DELAY 	1000
#define TASK_HK_LOW_RATE_DELAY 	10000

#define NUMBER_OF_SOLAR_PANNELS	6

#define ACK_HK_SIZE ACK_DATA_LENGTH
#define EPS_HK_SIZE 49
#define SP_HK_SIZE	FLOAT_SIZE * NUMBER_OF_SOLAR_PANNELS
#define CAM_HK_SIZE 62
#define COMM_HK_SIZE 12
#define ADCS_HK_SIZE 34

#define ADCS_SC_SIZE 6

#define ACK_FILE_NAME "ACKf"
#define EPS_HK_FILE_NAME "EPSf"
#define	SP_HK_FILE_NAME	"SPF"
#define CAM_HK_FILE_NAME "CAMf"
#define COMM_HK_FILE_NAME "COMMf"// TRX and ANTS HK
#define ADCS_HK_FILE_NAME "ADCf"// ADCS
#define BOS_HK_FILE_NAME	"BOSf"

typedef enum HK_dump_types{
	ACK_T = 0,
	EPS_HK_T = 1,
	CAMERA_HK_T = 2,
	COMM_HK_T = 3,
	ADCS_HK_T = 4,
	SP_HK_T = 5,
	this_is_not_the_file_you_are_looking_for = 18,
	ADCS_CSS_DATA_T = 20,
	ADCS_Magnetic_filed_T = 21,
	ADCS_CSS_sun_vector_T = 22,
	ADCS_wheel_speed_T = 23,
	ADCS_sensore_rate_T = 24,
	ADCS_MAG_CMD_T = 25,
	ADCS_wheel_CMD_T = 26,
	ADCS_Mag_raw_T = 27,
	ADCS_IGRF_MODEL_T = 28,
	ADCS_Gyro_BIAS_T = 29,
	ADCS_Inno_Vextor_T = 30,
	ADCS_Error_Vec_T = 31,
	ADCS_QUATERNION_COVARIANCE_T = 32,
	ADCS_ANGULAR_RATE_COVARIANCE_T = 33,
	ADCS_ESTIMATED_ANGLES_T = 34,
	ADCS_Estimated_AR_T = 35,
	ADCS_ECI_POS_T = 36,
	ADCS_ECI_VEL_T = 255,
	ADCS_SAV_Vel_T = 37,
	ADCS_ECEF_POS_T = 38,
	ADCS_LLH_POS_T = 39,
	ADCS_EST_QUATERNION_T = 40
}HK_types;

typedef union __attribute__ ((__packed__))
{
	byte raw[EPS_HK_SIZE];
	struct __attribute__((packed))
	{
		voltage_t photoVoltaic3;
		voltage_t photoVoltaic2;
		voltage_t photoVoltaic1;
		current_t photoCurrent3;
		current_t photoCurrent2;
		current_t photoCurrent1;
		current_t Total_photo_current;
		voltage_t VBatt;
		current_t Total_system_current;
		current_t currentChanel[6];
		short temp[6];
		unsigned int Number_of_EPS_reboots;
		unsigned char Cause_of_last_reset;
		unsigned char pptMode;
		byte channelStatus;
	}fields;
}EPS_HK;

typedef union __attribute__ ((__packed__))
{
	byte raw[CAM_HK_SIZE];
	struct __attribute__((packed))
	{
		voltage_t VoltageInput5V;
		current_t CurrentInput5V;
		voltage_t VoltageFPGA1V;
		current_t CurrentFPGA1V;
		voltage_t VoltageFPGA1V8;
		current_t CurrentFPGA1V8;
		voltage_t VoltageFPGA2V5;
		current_t CurrentFPGA2V5;
		voltage_t VoltageFPGA3V3;
		current_t CurrentFPGA3V3;
		voltage_t VoltageFlash1V8;
		current_t CurrentFlash1V8;
		voltage_t VoltageFlash3V3;
		current_t CurrentFlash3V3;
		voltage_t VoltageSNSR1V8;
		current_t CurrentSNSR1V8;
		voltage_t VoltageSNSRVDDPIX;
		current_t CurrentSNSRVDDPIX;
		voltage_t VoltageSNSR3V3;
		current_t CurrentSNSR3V3;
		voltage_t VoltageFlashVTT09;
		temp_t TempSMU3AB;
		temp_t TempSMU3BC;
		temp_t TempREGU6;
		temp_t TempREGU8;
		temp_t TempFlash;
	}fields;
}CAM_HK;

typedef union __attribute__ ((__packed__))
{
	byte raw[COMM_HK_SIZE];
	struct __attribute__((packed))
	{
		voltage_t bus_vol;
		current_t total_curr;
		unsigned short pa_temp;
		unsigned short locosc_temp;
		unsigned short ant_A_temp;
		unsigned short ant_B_temp;
	}fields;
}COMM_HK;

typedef union __attribute__ ((__packed__))
{
	byte raw[SP_HK_SIZE];
	struct __attribute__((packed))
	{
		int32_t SP_temp[NUMBER_OF_SOLAR_PANNELS];
	}fields;
}SP_HK;

typedef cspace_adcs_pwtempms_t ADCS_HK;


void HouseKeeping_highRate_Task();
void HouseKeeping_lowRate_Task();

int create_files(Boolean firstActivation);

int save_HK();

int build_HK_spl_packet(HK_types type, byte *raw_data, TM_spl *packet);

int save_ACK(Ack_type type, ERR_type err, command_id ACKcommandId);

int find_fileName(HK_types type, char *fileName);
int size_of_element(HK_types type);
#endif /* HOUSEKEEPING_H_ */
