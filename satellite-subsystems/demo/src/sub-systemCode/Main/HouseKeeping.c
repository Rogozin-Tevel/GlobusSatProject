/*


 * HouseKeeping.c
 *
 *  Created on: Jan 18, 2019
 *      Author: elain
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
#include <hcc/api_fat.h>

#include <hal/Storage/FRAM.h>

#include <string.h>

#include <satellite-subsystems/GomEPS.h>
#include <satellite-subsystems/SCS_Gecko/gecko_driver.h>
#include <satellite-subsystems/IsisAntS.h>
#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <satellite-subsystems/cspaceADCS.h>

#include "HouseKeeping.h"

#include "../COMM/splTypes.h"
#include "../Global/Global.h"
#include "../Global/sizes.h"
#include "../Global/FRAMadress.h"
#include "../Global/GlobalParam.h"
#include "../ADCS.h"



int find_fileName(HK_types type, char *fileName)
{
	switch(type)
	{
	case EPS_HK_T:
		strcpy(fileName, EPS_HK_FILE_NAME);
		break;
	case CAMERA_HK_T:
		strcpy(fileName, CAM_HK_FILE_NAME);
		break;
	case ACK_T:
		strcpy(fileName, ACK_FILE_NAME);
		break;
	case COMM_HK_T:
		strcpy(fileName, COMM_HK_FILE_NAME);
		break;
	case ADCS_HK_T:
		strcpy(fileName, ADCS_HK_FILE_NAME);
		break;
	case SP_HK_T:
		strcpy(fileName, SP_HK_FILE_NAME);
		break;
	case ADCS_CSS_DATA_T:
		strcpy(fileName, NAME_OF_CSS_DATA_FILE);
		break;
	case ADCS_Magnetic_filed_T:
		strcpy(fileName, NAME_OF_MAGNETIC_FIELD_DATA_FILE);
		break;
	case ADCS_CSS_sun_vector_T:
		strcpy(fileName, CSS_SUN_VECTOR_DATA_FILE);
		break;
	case ADCS_wheel_speed_T:
		strcpy(fileName, WHEEL_SPEED_DATA_FILE);
		break;
	case ADCS_sensore_rate_T:
		strcpy(fileName, SENSOR_RATE_DATA_FILE);
		break;
	case ADCS_MAG_CMD_T:
		strcpy(fileName, MAGNETORQUER_CMD_FILE);
		break;
	case ADCS_wheel_CMD_T:
		strcpy(fileName, WHEEL_SPEED_CMD_FILE);
		break;
	case ADCS_Mag_raw_T:
		strcpy(fileName, RAW_MAGNETOMETER_FILE);
		break;
	case ADCS_IGRF_MODEL_T:
		strcpy(fileName, IGRF_MODEL_MAGNETIC_FIELD_VECTOR_FILE);
		break;
	case ADCS_Gyro_BIAS_T:
		strcpy(fileName, GYRO_BIAS_FILE);
		break;
	case ADCS_Inno_Vextor_T:
		strcpy(fileName, INNOVATION_VECTOR_FILE);
		break;
	case ADCS_Error_Vec_T:
		strcpy(fileName, ERROR_VECTOR_FILE);
		break;
	case ADCS_QUATERNION_COVARIANCE_T:
		strcpy(fileName, QUATERNION_COVARIANCE_FILE);
		break;
	case ADCS_ANGULAR_RATE_COVARIANCE_T:
		strcpy(fileName, ANGULAR_RATE_COVARIANCE_FILE);
		break;
	case ADCS_ESTIMATED_ANGLES_T:
		strcpy(fileName, ESTIMATED_ANGLES_FILE);
		break;
	case ADCS_Estimated_AR_T:
		strcpy(fileName, ESTIMATED_ANGULAR_RATE_FILE);
		break;
	case ADCS_ECI_POS_T:
		strcpy(fileName, NAME_OF_ECI_POSITION_DATA_FILE);
		break;
	case ADCS_SAV_Vel_T:
		strcpy(fileName, NAME_OF_SATALLITE_VELOCITY_DATA_FILE);
		break;
	case ADCS_ECEF_POS_T:
		strcpy(fileName, NAME_OF_ECEF_POSITION_DATA_FILE);
		break;
	case ADCS_LLH_POS_T:
		strcpy(fileName, NAME_OF_LLH_POSTION_DATA_FILE);
		break;
	case ADCS_EST_QUATERNION_T:
		strcpy(fileName, ESTIMATED_QUATERNION_FILE);
		break;
	default:
		return -1;
		break;
	}
	return 0;
}
int size_of_element(HK_types type)
{
	if (type == EPS_HK_T)
		return EPS_HK_SIZE;
	if (type == CAMERA_HK_T)
		return CAM_HK_SIZE;
	if (type == ACK_T)
		return ACK_DATA_LENGTH;
	if (type == SP_HK_T)
		return SP_HK_SIZE;
	if (type == COMM_HK_T)
		return COMM_HK_SIZE;
	if (type == ADCS_HK_T)
		return ADCS_HK_SIZE;
	if (ADCS_CSS_DATA_T <= type && type <= ADCS_EST_QUATERNION_T)
		return ADCS_SC_SIZE;
	if (ADCS_ECI_VEL_T == type)
		return ADCS_SC_SIZE;

	return 0;
}

int save_ACK(Ack_type type, ERR_type err, command_id ACKcommandId)
{
	byte raw_ACK[ACK_DATA_LENGTH];
	build_data_field_ACK(type, err, ACKcommandId, raw_ACK);
	FileSystemResult error = c_fileWrite(ACK_FILE_NAME, raw_ACK);
	if (error != FS_SUCCSESS)
	{
		printf("could not save ACK, error %d", error);
		//if theres errors
	}

#ifdef TESTING
	printf("ACK///id: %u, type: %u, err: %u\n", ACKcommandId, type, err);
#endif
	return 0;
}

void set_GP_EPS(EPS_HK hk_in)
{
	set_Vbatt(hk_in.fields.VBatt);
	set_curBat(hk_in.fields.Total_system_current);
	for (int i = 0; i < 4; i++)
	{
		set_tempEPS(i, hk_in.fields.temp[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		set_tempBatt(i, hk_in.fields.temp[4 + i]);
	}
	set_cur3V3(hk_in.fields.currentChanel[0]);
	set_cur5V(hk_in.fields.currentChanel[3]);
}
void set_GP_COMM(ISIStrxvuRxTelemetry_revC hk_in)
{
	set_tempComm_LO(hk_in.fields.locosc_temp);
	set_tempComm_PA(hk_in.fields.pa_temp);
	set_RxDoppler(hk_in.fields.rx_doppler);
	set_RxRSSI(hk_in.fields.rx_rssi);
	ISIStrxvuTxTelemetry_revC tx_tm;
	int err = IsisTrxvu_tcGetTelemetryAll_revC(0, &tx_tm);
	check_int("set_GP_COMM, IsisTrxvu_tcGetTelemetryAll_revC", err);
	set_TxForw(tx_tm.fields.tx_fwrdpwr);
	set_TxRefl(tx_tm.fields.tx_reflpwr);
}


int SP_HK_collect(SP_HK* hk_out)
{
	int errors[NUMBER_OF_SOLAR_PANNELS];
	int error_combine = 1;
	IsisSolarPanelv2_wakeup();
	int32_t paneltemp;
	uint8_t status = 0;
	for(int i = 0; i < NUMBER_OF_SOLAR_PANNELS; i++)
	{
		errors[i] = IsisSolarPanelv2_getTemperature(i, &paneltemp, &status);
		check_int("EPS_HK_collect, IsisSolarPanelv2_getTemperature", errors[i]);
		hk_out->fields.SP_temp[i] = paneltemp;
		error_combine &= errors[i];
	}
	IsisSolarPanelv2_sleep();
	return error_combine;
}
int EPS_HK_collect(EPS_HK* hk_out)
{
	gom_eps_hk_t gom_hk;
	int error = GomEpsGetHkData_general(0, &gom_hk);
	check_int("EPS_HK_collect, GomEpsGetHkData_param", error);

	hk_out->fields.photoVoltaic3 = gom_hk.fields.vboost[2];
	hk_out->fields.photoVoltaic2 = gom_hk.fields.vboost[1];
	hk_out->fields.photoVoltaic1 = gom_hk.fields.vboost[0];
	hk_out->fields.photoCurrent3 = gom_hk.fields.curin[2];
	hk_out->fields.photoCurrent2 = gom_hk.fields.curin[1];
	hk_out->fields.photoCurrent1 = gom_hk.fields.curin[0];
	hk_out->fields.Total_photo_current = gom_hk.fields.cursun;
	hk_out->fields.VBatt = gom_hk.fields.vbatt;
	hk_out->fields.Total_system_current = gom_hk.fields.cursys;
	int i;
	for (i = 0; i < 6; i++)
	{
		hk_out->fields.currentChanel[i] = gom_hk.fields.curout[i];
		hk_out->fields.temp[i] = gom_hk.fields.temp[i];
	}
	hk_out->fields.Cause_of_last_reset = gom_hk.fields.bootcause;
	hk_out->fields.Number_of_EPS_reboots = gom_hk.fields.counter_boot;
	hk_out->fields.pptMode = gom_hk.fields.pptmode;
	hk_out->fields.channelStatus = 0;
	for (i = 0; i < 8; i++)
	{
		hk_out->fields.channelStatus += (byte)(1 >> gom_hk.fields.output[i]);
	}

	set_GP_EPS(*hk_out);
	return error;
}
int CAM_HK_collect(CAM_HK* hk_out)
{
	if (get_system_state(cam_param) == SWITCH_OFF)
		return -1;
	hk_out->fields.VoltageInput5V = (voltage_t)(GECKO_GetCurrentInput5V() * 1000);
	hk_out->fields.CurrentInput5V = (current_t)(GECKO_GetVoltageInput5V() * 1000);
	hk_out->fields.VoltageFPGA1V = (voltage_t)(GECKO_GetVoltageFPGA1V() * 1000);
	hk_out->fields.CurrentFPGA1V = (current_t)(GECKO_GetCurrentFPGA1V() * 1000);
	hk_out->fields.VoltageFPGA1V8 = (voltage_t)(GECKO_GetVoltageFPGA1V8() * 1000);
	hk_out->fields.CurrentFPGA1V8 = (current_t)(GECKO_GetCurrentFPGA1V8() * 1000);
	hk_out->fields.VoltageFPGA2V5 = (voltage_t)(GECKO_GetVoltageFPGA2V5() * 1000);
	hk_out->fields.CurrentFPGA2V5 = (current_t)(GECKO_GetCurrentFPGA2V5() * 1000);
	hk_out->fields.VoltageFPGA3V3 = (voltage_t)(GECKO_GetVoltageFPGA3V3() * 1000);
	hk_out->fields.CurrentFPGA3V3 = (current_t)(GECKO_GetCurrentFPGA3V3() * 1000);
	hk_out->fields.VoltageFlash1V8 = (voltage_t)(GECKO_GetVoltageFlash1V8() * 1000);
	hk_out->fields.CurrentFlash1V8 = (current_t)(GECKO_GetCurrentFlash1V8() * 1000);
	hk_out->fields.VoltageFlash3V3 = (voltage_t)(GECKO_GetVoltageFlash3V3() * 1000);
	hk_out->fields.CurrentFlash3V3 = (current_t)(GECKO_GetCurrentFlash3V3() * 1000);
	hk_out->fields.VoltageSNSR1V8 = (voltage_t)(GECKO_GetVoltageSNSR1V8() * 1000);
	hk_out->fields.CurrentSNSR1V8 = (current_t)(GECKO_GetCurrentSNSR1V8() * 1000);
	hk_out->fields.VoltageSNSRVDDPIX = (voltage_t)(GECKO_GetVoltageSNSRVDDPIX() * 1000);
	hk_out->fields.CurrentSNSRVDDPIX = (current_t)(GECKO_GetCurrentSNSRVDDPIX() * 1000);
	hk_out->fields.VoltageSNSR3V3 = (voltage_t)(GECKO_GetVoltageSNSR3V3() * 1000);
	hk_out->fields.CurrentSNSR3V3 = (current_t)(GECKO_GetCurrentSNSR3V3() * 1000);
	hk_out->fields.VoltageFlashVTT09 = (voltage_t)(GECKO_GetVoltageFlashVTT09() * 1000);

	hk_out->fields.TempSMU3AB = GECKO_GetTempSMU3AB();
	hk_out->fields.TempSMU3BC = GECKO_GetTempSMU3BC();
	hk_out->fields.TempREGU6 = GECKO_GetTempREGU6();
	hk_out->fields.TempREGU8 = GECKO_GetTempREGU8();
	hk_out->fields.TempFlash = GECKO_GetTempFlash();

	return 0;
}
int COMM_HK_collect(COMM_HK* hk_out)
{
	ISIStrxvuRxTelemetry_revC telemetry;
	int error_trxvu = -1, error_antA = -1, error_antB = -1;
	error_trxvu = IsisTrxvu_rcGetTelemetryAll_revC(0, &telemetry);
	check_int("COMM_HK_collect, IsisTrxvu_rcGetTelemetryAll_revC", error_trxvu);
	hk_out->fields.bus_vol = telemetry.fields.bus_volt;
	hk_out->fields.total_curr = telemetry.fields.total_current;
	hk_out->fields.pa_temp = telemetry.fields.pa_temp;
	hk_out->fields.locosc_temp = telemetry.fields.locosc_temp;

#ifdef ANTS_ON
	error_antA = IsisAntS_getTemperature(0, isisants_sideA, &(hk_out->fields.ant_A_temp));
	check_int("COMM_HK_collect ,IsisAntS_getTemperature", error_antA);

	error_antB = IsisAntS_getTemperature(0, isisants_sideB, &(hk_out->fields.ant_B_temp));
	check_int("COMM_HK_collect ,IsisAntS_getTemperature", error_antB);
#endif

	set_GP_COMM(telemetry);

	return (error_trxvu && error_antA && error_antB);
}
int ADCS_HK_collect(ADCS_HK* hk_out)
{
	int error = cspaceADCS_getPowTempMeasTLM(0, hk_out);
	check_int("ADCS_HK_collect, cspaceADCS_getPowTempMeasTLM", error);
	return error;
}


int COMM_create_file()
{
	int er = c_fileReset(COMM_HK_FILE_NAME);
	check_int("delete_comm_file, f_delete", er);
	FileSystemResult error = c_fileCreate(COMM_HK_FILE_NAME, COMM_HK_SIZE);
	if (error != FS_SUCCSESS)
	{
		printf("ERROR IN CREATING ACK FILE %d\n", error);
		//error
	}
	return 0;
}
int ACK_create_file()
{
	int er = c_fileReset(ACK_FILE_NAME);
	check_int("delete_ack_file, f_delete", er);
	FileSystemResult error = c_fileCreate(ACK_FILE_NAME, ACK_HK_SIZE);
	if (error != FS_SUCCSESS)
	{
		printf("ERROR IN CREATING ACK FILE %d\n", error);
		//error
	}
	return 0;
}
int EPS_create_file()
{
	int er = c_fileReset(EPS_HK_FILE_NAME);
	check_int("delete_eps_file, f_delete", er);
	FileSystemResult error = c_fileCreate(EPS_HK_FILE_NAME, EPS_HK_SIZE);
	if (error != FS_SUCCSESS)
	{
		printf("ERROR IN CREATING EPS FILE %d\n", error);
		//error
	}
	return 0;
}
int CAM_create_file()
{
	int er = c_fileReset(CAM_HK_FILE_NAME);
	check_int("delete_comm_file, f_delete", er);
	FileSystemResult error = c_fileCreate(CAM_HK_FILE_NAME, CAM_HK_SIZE);
	if (error != FS_SUCCSESS)
	{
		//error
	}
	return 0;
}
int ADCS_create_file()
{
	int er = c_fileReset(ADCS_HK_FILE_NAME);
	check_int("delete_comm_file, f_delete", er);
	FileSystemResult error = c_fileCreate(ADCS_HK_FILE_NAME, ADCS_HK_SIZE);
	if (error != FS_SUCCSESS)
	{
		//error
	}
	return 0;
}
int SP_create_file()
{
	int er = c_fileReset(SP_HK_FILE_NAME);
	check_int("delete_comm_file, f_delete", er);
	FileSystemResult error = c_fileCreate(SP_HK_FILE_NAME, SP_HK_SIZE);
	if (error != FS_SUCCSESS)
	{
		//error
	}
	return 0;
}

int create_files(Boolean firstActivation)
{
	if (!firstActivation)
		return -1;

	ACK_create_file();
	EPS_create_file();
	CAM_create_file();
	COMM_create_file();
	//ADCS_CreateFiles();
	SP_create_file();
	return 0;
}

void save_SP_HK()
{
	int error;
	SP_HK eps_hk;
	// 1.1. collect hk
	error = SP_HK_collect(&eps_hk);
	if (error == 0)
	{
		// 1.2. save hk in filesystem
		error = c_fileWrite(SP_HK_FILE_NAME, (void*)(&eps_hk));
		if (error == FS_NOT_EXIST)
		{
			// 1.3. create file if not exist
			EPS_create_file();
		}
	}
	else
	{
		//can't get eps hk
		printf("ERROR IN COLLECTING SP TM: %d\n", error);
	}
}
void save_EPS_HK()
{
	int error;
	EPS_HK eps_hk;
	// 1.1. collect hk
	error = EPS_HK_collect(&eps_hk);
	if (error == 0)
	{
		// 1.2. save hk in filesystem
		error = c_fileWrite(EPS_HK_FILE_NAME, (void*)(&eps_hk));
		if (error == FS_NOT_EXIST)
		{
			// 1.3. create file if not exist
			EPS_create_file();
		}
	}
	else
	{
		//can't get eps hk
		printf("ERROR IN COLLECTING EPS TM: %d\n", error);
	}
}
void save_CAM_HK()
{
	int error;
	if (get_system_state(cam_operational_param))
	{
		CAM_HK cam_hk;
		// 2.1. collect hk
		error = CAM_HK_collect(&cam_hk);
		if (error == 0)
		{
			// 2.2. save hk in filesystem
			error = c_fileWrite(CAM_HK_FILE_NAME, (void*)(&cam_hk));
			if (error == FS_NOT_EXIST)
			{
				// 2.3. create file if not exist
				CAM_create_file();
			}
		}
		else
		{
			//can't get eps hk
			//printf("ERROR IN COLLECTING CAM TM: %d\n", error);
		}
	}
}
void save_COMM_HK()
{
	int error;
	COMM_HK comm_hk;
	// 3.1. collect hk
	error = COMM_HK_collect(&comm_hk);
	if (error == 0)
	{
		// 3.2. save hk in filesystem
		error = c_fileWrite(COMM_HK_FILE_NAME, (void*)(&comm_hk));
		if (error == FS_NOT_EXIST)
		{
			// 3.3. create file if not exist
			COMM_create_file();
		}
	}
	else
	{
		//can't get eps hk
		printf("ERROR IN COLLECTING COMM TM: %d\n", error);
	}
}
void save_ADCS_HK()
{
	int error;
	ADCS_HK adcs_hk;
	// 3.1. collect hk
	if (get_system_state(ADCS_param))
	{
		error = ADCS_HK_collect(&adcs_hk);
		if (error == 0)
		{
			// 3.2. save hk in filesystem
			error = c_fileWrite(ADCS_HK_FILE_NAME, (void*)(&adcs_hk));
			if (error == FS_NOT_EXIST)
			{
				// 3.3. create file if not exist
				ADCS_create_file();
			}
		}
		else
		{
			//can't get eps hk
			printf("ERROR IN COLLECTING ADCS TM: %d\n", error);
		}
	}
}

int EPS_HK_raw_BigEnE(byte* raw_in, byte* raw_out)
{
	int params[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 1, 1, 1};
	int lastPlace = 0, l;
	for (int i = 0; i < 25; i++)
	{
		for (l = 0; l < params[i] / 2; l++)
		{
			raw_out[l + lastPlace] = raw_in[lastPlace + params[i] - 1 - l];
			raw_out[lastPlace + params[i] - 1 - l] = raw_in[l + lastPlace];
		}
		lastPlace += params[i];
	}

	return 0;
}
int CAM_HK_raw_BigEnE(byte* raw_in, byte* raw_out)
{
	int params[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5};
	int lastPlace = 0, l;
	for (int i = 0; i < 26; i++)
	{
		for (l = 0; l < params[i] / 2; l++)
		{
			raw_out[l + lastPlace] = raw_in[lastPlace + params[i] - 1 - l];
			raw_out[lastPlace + params[i] - 1 - l] = raw_in[l + lastPlace];
		}
		lastPlace += params[i];
	}

	return 0;
}
int COMM_HK_raw_BigEnE(byte* raw_in, byte* raw_out)
{
	int params[] = {2, 2, 2, 2, 2, 2};
	int lastPlace = 0, l;
	for (int i = 0; i < 6; i++)
	{
		for (l = 0; l < params[i] / 2; l++)
		{
			raw_out[l + lastPlace] = raw_in[lastPlace + params[i] - 1 - l];
			raw_out[lastPlace + params[i] - 1 - l] = raw_in[l + lastPlace];
		}
		lastPlace += params[i];
	}

	return 0;
}
int ADCS_HK_raw_BigEnE(byte* raw_in, byte* raw_out)
{
	int params[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int lastPlace = 0, l;
	for (int i = 0; i < 3; i++)
	{
		for (l = 0; l < params[i] / 2; l++)
		{
			raw_out[l + lastPlace] = raw_in[lastPlace + params[i] - 1 - l];
			raw_out[lastPlace + params[i] - 1 - l] = raw_in[l + lastPlace];
		}
		lastPlace += params[i];
	}

	return 0;
}
int SP_HK_raw_BiEnE(byte* raw_in, byte* raw_out)
{
	int params[] = {4, 4, 4, 4, 4, 4};
	int lastPlace = 0, l;
	for (int i = 0; i < 6; i++)
	{
		for (l = 0; l < params[i] / 2; l++)
		{
			raw_out[l + lastPlace] = raw_in[lastPlace + params[i] - 1 - l];
			raw_out[lastPlace + params[i] - 1 - l] = raw_in[l + lastPlace];
		}
		lastPlace += params[i];
	}

	return 0;
}
int ADCS_SC_raw_BigEnE(byte* raw_in, byte* raw_out)
{
	int params[] = {2, 2, 2};
	int lastPlace = 0, l;
	for (int i = 0; i < 3; i++)
	{
		for (l = 0; l < params[i] / 2; l++)
		{
			raw_out[l + lastPlace] = raw_in[lastPlace + params[i] - 1 - l];
			raw_out[lastPlace + params[i] - 1 - l] = raw_in[l + lastPlace];
		}
		lastPlace += params[i];
	}

	return 0;
}


int build_HK_spl_packet(HK_types type, byte *raw_data, TM_spl *packet)
{
	time_unix data_time;
	memcpy(&data_time, raw_data, sizeof(time_unix));
	switch(type)
	{
	case ACK_T:
		packet->type = ACK_TYPE;
		packet->subType = ACK_ST;
		packet->length = ACK_DATA_LENGTH;
		packet->time = data_time;
		memcpy(packet->data, raw_data + TIME_SIZE, ACK_DATA_LENGTH);

		break;
	case EPS_HK_T:
		packet->type = DUMP_T;
		packet->subType = EPS_DUMP_ST;
		packet->length = EPS_HK_SIZE;
		packet->time = data_time;
		EPS_HK_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case CAMERA_HK_T:
		packet->type = DUMP_T;
		packet->subType = CAM_DUMP_ST;
		packet->length = CAM_HK_SIZE;
		packet->time = data_time;
		CAM_HK_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case COMM_HK_T:
		packet->type = DUMP_T;
		packet->subType = COMM_DUMP_ST;
		packet->length = COMM_HK_SIZE;
		packet->time = data_time;
		COMM_HK_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_HK_T:
		packet->type = DUMP_T;
		packet->subType = ADCS_DUMP_ST;
		packet->length = ADCS_HK_SIZE;
		packet->time = data_time;
		ADCS_HK_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case SP_HK_T:
		packet->type = DUMP_T;
		packet->subType = SP_DUMP_ST;
		packet->length = SP_HK_SIZE;
		packet->time = data_time;
		ADCS_HK_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_CSS_DATA_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_CSS_DATA_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_Magnetic_filed_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_MAGNETIC_FILED_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_CSS_sun_vector_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_CSS_SUN_VECTOR_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_wheel_speed_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_WHEEL_SPEED_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_sensore_rate_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_SENSORE_RATE_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_MAG_CMD_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_MAG_CMD_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_wheel_CMD_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_WHEEL_CMD_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_Mag_raw_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_MAG_RAW_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_IGRF_MODEL_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_IGRF_MODEL_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_Gyro_BIAS_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_GYRO_BIAS_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_Inno_Vextor_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_INNO_VEXTOR_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_Error_Vec_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_ERROR_VEC_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_QUATERNION_COVARIANCE_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_QUATERNION_COVARIANCE_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_ANGULAR_RATE_COVARIANCE_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_ANGULAR_RATE_COVARIANCE_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_ESTIMATED_ANGLES_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_ESTIMATED_ANGLES_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_Estimated_AR_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_ESTIMATED_AR_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_ECI_POS_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_ECI_POS_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_SAV_Vel_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_SAV_VEL_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_ECEF_POS_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_ECEF_POS_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_LLH_POS_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_LLH_POS_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	case ADCS_EST_QUATERNION_T:
		packet->type = ADCS_SC_ST;
		packet->subType = ADCS_EST_QUATERNION_ST;
		packet->length = ADCS_SC_SIZE;
		packet->time = data_time;
		ADCS_SC_raw_BigEnE((raw_data + TIME_SIZE), packet->data);
		break;
	default:
		return -1;
		break;
	}
	return 0;
}


void HouseKeeping_highRate_Task()
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	const portTickType xFrequency = TASK_HK_HIGH_RATE_DELAY;
	while(1)
	{
		byte DF;
		FRAM_write(&DF, STOP_TELEMETRY_ADDR, 1);
		if (DF != TRUE_8BIT)
		{
			save_EPS_HK();

			save_CAM_HK();

			save_COMM_HK();

			//save_ADCS_HK();
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void HouseKeeping_lowRate_Task()
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	const portTickType xFrequency = TASK_HK_LOW_RATE_DELAY;
	while(1)
	{
		byte DF;
		FRAM_write(&DF, STOP_TELEMETRY_ADDR, 1);
		if (DF != TRUE_8BIT)
		{
			save_SP_HK();
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
