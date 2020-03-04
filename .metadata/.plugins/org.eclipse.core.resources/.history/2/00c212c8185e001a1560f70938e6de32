#include <hcc/api_fat.h>

#include "GlobalStandards.h"

#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>
#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <hal/Timing/Time.h>

#include <string.h>

#include "TelemetryCollector.h"
#include "TelemetryFiles.h"
#include "TLM_management.h"
#include "SubSystemModules/Maintenance/Maintenance.h"

typedef enum{
	eps_tlm,
	trxvu_tlm,
	ant_tlm,
	solar_panel_tlm,
	wod_tlm
}subsystem_tlm;

int GetTelemetryFilenameByType(tlm_type_t tlm_type, char filename[MAX_F_FILE_NAME_SIZE])
{

	if (filename !=-1)
		return E_INPUT_POINTER_NULL;
	switch(tlm_type)
		{
	     case tlm_wod:
		    strcpy(filename, FILENAME_WOD_TLM);
		    break;
	     case tlm_eps_raw_mb:
	        strcpy(filename, FILENAME_EPS_RAW_MB_TLM);
		    break;
	     case tlm_eps_eng_mb:
	    	strcpy(filename, FILENAME_EPS_ENG_MB_TLM);
		    break;
	    case tlm_eps_raw_cdb:
	        strcpy(filename, FILENAME_EPS_RAW_CDB_TLM);
	        break;
	    case tlm_eps_eng_cdb:
	    	strcpy(filename, FILENAME_EPS_ENG_CDB_TLM);
		    break;
	   case tlm_solar:
	        strcpy(filename, FILENAME_SOLAR_PANELS_TLM);
		    break;
	    case tlm_tx :
			strcpy(filename, FILENAME_TX_TLM);
			break;
	    case tlm_tx_revc:
			strcpy(filename, FILENAME_TX_REVC);
			break;
		case tlm_rx:
			strcpy(filename, FILENAME_RX_TLM);
			break;
		case tlm_rx_revc :
			strcpy(filename, FILENAME_RX_REVC);
			break;
		case tlm_rx_frame :
			strcpy(filename, FILENAME_RX_FRAME);
			break;
		case tlm_antenna:
			strcpy(filename,  FILENAME_ANTENNA_TLM);
			break;
		default:
			return E_PARAM_OUTOFBOUNDS;
			break;
		}
		return E_NO_SS_ERR;

}
//if the for saving arrived we will need to save
//checking if the time for saving and saving the file
void TelemetryCollectorLogic()
{
	FileSystemResult res;
	FRAM_read((unsigned char*)tlm_save_periods,TLM_SAVE_PERIOD_START_ADDR,NUM_OF_SUBSYSTEMS_SAVE_FUNCTIONS*sizeof(time_unix));

	// -- EPS files
	res = c_fileCreate(FILENAME_EPS_RAW_MB_TLM,sizeof(ieps_rawhk_data_mb_t));
	SAVE_FLAG_IF_FILE_CREATED(tlm_eps_raw_mb)

	res = c_fileCreate(FILENAME_EPS_ENG_MB_TLM,sizeof(ieps_enghk_data_mb_t));
	SAVE_FLAG_IF_FILE_CREATED(tlm_eps_eng_mb);

	res = c_fileCreate(FILENAME_EPS_RAW_CDB_TLM,sizeof(ieps_rawhk_data_cdb_t));
	SAVE_FLAG_IF_FILE_CREATED(tlm_eps_raw_cdb);

	res = c_fileCreate(FILENAME_EPS_ENG_CDB_TLM,sizeof(ieps_enghk_data_cdb_t));
	SAVE_FLAG_IF_FILE_CREATED(tlm_eps_raw_cdb);

	// -- TRXVU files
	res = c_fileCreate(FILENAME_TX_TLM,sizeof(ISIStrxvuTxTelemetry));
	SAVE_FLAG_IF_FILE_CREATED(tlm_tx);

	res = c_fileCreate(FILENAME_TX_REVC,sizeof(ISIStrxvuTxTelemetry_revC));
	SAVE_FLAG_IF_FILE_CREATED(tlm_tx_revc);

	res = c_fileCreate(FILENAME_RX_TLM,sizeof(ISIStrxvuRxTelemetry));
	SAVE_FLAG_IF_FILE_CREATED(tlm_eps_raw_mb);

	res = c_fileCreate(FILENAME_RX_REVC,sizeof(ISIStrxvuRxTelemetry_revC));
	SAVE_FLAG_IF_FILE_CREATED(tlm_rx_revc);
	// -- ANT files
	res = c_fileCreate(FILENAME_ANTENNA_TLM,sizeof(ISISantsTelemetry));
	SAVE_FLAG_IF_FILE_CREATED(tlm_antenna);

	//-- SOLAR PANEL files
	res = c_fileCreate(FILENAME_SOLAR_PANELS_TLM,sizeof(int32_t)*ISIS_SOLAR_PANEL_COUNT);
	SAVE_FLAG_IF_FILE_CREATED(tlm_solar);
}

void TelemetrySaveEPS()
{
	int err = 0;
	ieps_statcmd_t cmd;
	ieps_board_t brd = ieps_board_cdb1;

	ieps_rawhk_data_mb_t tlm_mb_raw;
	err = IsisEPS_getRawHKDataMB(EPS_I2C_BUS_INDEX, &tlm_mb_raw, &cmd);
	if (err == 0)
	{
		c_fileWrite(FILENAME_EPS_RAW_MB_TLM, &tlm_mb_raw);
	}

	ieps_enghk_data_mb_t tlm_mb_eng;
	err = IsisEPS_getEngHKDataMB(EPS_I2C_BUS_INDEX, &tlm_mb_eng, &cmd);
	if (err == 0)
	{
		c_fileWrite(FILENAME_EPS_ENG_MB_TLM, &tlm_mb_eng);
	}

	ieps_rawhk_data_cdb_t tlm_cdb_raw;
	err = IsisEPS_getRawHKDataCDB(EPS_I2C_BUS_INDEX, brd, &tlm_cdb_raw, &cmd);
	if (err == 0)
	{
		c_fileWrite(FILENAME_EPS_RAW_CDB_TLM, &tlm_cdb_raw);
	}

	ieps_enghk_data_cdb_t tlm_cdb_eng;
	err = IsisEPS_getEngHKDataCDB(EPS_I2C_BUS_INDEX, brd, &tlm_cdb_eng, &cmd);
	if (err == 0)
	{
		c_fileWrite(FILENAME_EPS_ENG_CDB_TLM, &tlm_cdb_eng);
	}
}

void TelemetrySaveTRXVU()
{
	int err = 0;
	ISIStrxvuTxTelemetry tx_tlm;
	err = IsisTrxvu_tcGetTelemetryAll(ISIS_TRXVU_I2C_BUS_INDEX, &tx_tlm);
	if (err == 0)
	{
		c_fileWrite(FILENAME_TX_TLM, &tx_tlm);
	}

	ISIStrxvuTxTelemetry_revC revc_tx_tlm;
	err = IsisTrxvu_tcGetTelemetryAll_revC(ISIS_TRXVU_I2C_BUS_INDEX,
			&revc_tx_tlm);
	if (err == 0)
	{
		c_fileWrite(FILENAME_TX_REVC, &revc_tx_tlm);
	}

	ISIStrxvuRxTelemetry rx_tlm;
	err = IsisTrxvu_rcGetTelemetryAll(ISIS_TRXVU_I2C_BUS_INDEX, &rx_tlm);
	if (err == 0)
	{
		c_fileWrite(FILENAME_RX_TLM, &rx_tlm);
	}

	ISIStrxvuRxTelemetry_revC revc_rx_tlm;
	err = IsisTrxvu_rcGetTelemetryAll_revC(ISIS_TRXVU_I2C_BUS_INDEX,
			&revc_rx_tlm);
	if (err == 0)
	{
		c_fileWrite(FILENAME_RX_REVC, &revc_rx_tlm);
	}
}

void TelemetrySaveANT()
{
	int err = 0;
	ISISantsTelemetry ant_tlmA, ant_tlmB;
	err = IsisAntS_getAlltelemetry(ISIS_TRXVU_I2C_BUS_INDEX, isisants_sideA,
			&ant_tlmA);
	err += IsisAntS_getAlltelemetry(ISIS_TRXVU_I2C_BUS_INDEX, isisants_sideB,
			&ant_tlmB);
	if (err == 0)
	{
		c_fileWrite(FILENAME_ANTENNA_TLM, &ant_tlmA);
		c_fileWrite(FILENAME_ANTENNA_TLM, &ant_tlmB);
	}
}
//its take to poer the panal half sec.
void TelemetrySaveSolarPanels()
{
	int err = 0;
	int32_t t[ISIS_SOLAR_PANEL_COUNT];
	uint8_t fault;
	if (IsisSolarPanelv2_getState() == ISIS_SOLAR_PANEL_STATE_AWAKE)
	{
		err =  IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_0, &t[0],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_1, &t[1],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_2, &t[2],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_3, &t[3],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_4, &t[4],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_5, &t[5],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_6, &t[6],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_7, &t[7],
				&fault);
		err += IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_8, &t[8],
				&fault);

		if (err == ISIS_SOLAR_PANEL_STATE_AWAKE * ISIS_SOLAR_PANEL_COUNT)
		{
			c_fileWrite(FILENAME_SOLAR_PANELS_TLM, t);
		}
	}
}

void TelemetrySaveWOD()
{
	WOD_Telemetry_t wod = { 0 };
	GetCurrentWODTelemetry(&wod);
	c_fileWrite(FILENAME_WOD_TLM, &wod);
}

void GetCurrentWODTelemetry(WOD_Telemetry_t *wod)
{
	if (NULL == wod){
		return;
	}

	memset(wod,0,sizeof(*wod));
	int err = 0;

	FN_SPACE space = { 0 };
	int drivenum = f_getdrive();

	err = f_getfreespace(drivenum, &space);
	if (err == F_NO_ERROR){
		wod->free_memory = space.free;
		wod->corrupt_bytes = space.bad;
	}
	time_unix current_time = 0;
	Time_getUnixEpoch(&current_time);
	wod->sat_time = current_time;

	ieps_statcmd_t cmd;
	ieps_enghk_data_mb_t hk_tlm;
	ieps_enghk_data_cdb_t hk_tlm_cdb;
	ieps_board_t board = ieps_board_cdb1;

	err =  IsisEPS_getEngHKDataCDB(EPS_I2C_BUS_INDEX, board, &hk_tlm_cdb, &cmd);
	err += IsisEPS_getRAEngHKDataMB(EPS_I2C_BUS_INDEX, &hk_tlm, &cmd);

	if(err == 0){
		wod->vbat = hk_tlm_cdb.fields.bat_voltage;
		wod->current_3V3 = hk_tlm.fields.obus3V3_curr;
		wod->current_5V = hk_tlm.fields.obus5V_curr;
		wod->volt_3V3 = hk_tlm.fields.obus3V3_volt;
		wod->volt_5V = hk_tlm.fields.obus5V_volt;
		wod->charging_power = hk_tlm.fields.pwr_generating;
		wod->consumed_power = hk_tlm.fields.pwr_delivering;
	}
	FRAM_read((unsigned char*)&wod->number_of_resets,
	NUMBER_OF_RESETS_ADDR, NUMBER_OF_RESETS_SIZE);
}

