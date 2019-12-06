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
}

void TelemetrySaveEPS()
{
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
}
//its take to poer the panal half sec.
void TelemetrySaveSolarPanels()
{
}

void TelemetrySaveWOD()
{
}

void GetCurrentWODTelemetry(WOD_Telemetry_t *wod)
{
}

