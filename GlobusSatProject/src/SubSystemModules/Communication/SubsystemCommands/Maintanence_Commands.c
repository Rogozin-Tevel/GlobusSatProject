#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "GlobalStandards.h"

#include <hal/Timing/Time.h>

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>
#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif


#include <hcc/api_fat.h>
#include <hal/Drivers/I2C.h>
#include <stdlib.h>
#include <string.h>
#include <hal/errors.h>
#include "TLM_management.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "Maintanence_Commands.h"

int CMD_GenericI2C(sat_packet_t *cmd)
{
	return 0;
}

int CMD_FRAM_ReadAndTransmitt(sat_packet_t *cmd)
{
	return 0;
}

int CMD_FRAM_WriteAndTransmitt(sat_packet_t *cmd)
{
	return 0;
}

int CMD_FRAM_Start(sat_packet_t *cmd)
{
	return 0;
}

int CMD_FRAM_Stop(sat_packet_t *cmd)
{
	return 0;
}

int CMD_FRAM_GetDeviceID(sat_packet_t *cmd)
{
	return 0;
}

int CMD_UpdateSatTime(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetSatTime(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetSatUptime(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SoftTRXVU_ComponenetReset(sat_packet_t *cmd)
{
	return 0;
}

int CMD_HardTRXVU_ComponenetReset(sat_packet_t *cmd)
{
	return 0;
}

int CMD_AntennaDeploy(sat_packet_t *cmd)
{
	return 0;
}

int CMD_ResetComponent(reset_type_t rst_type)
{
	return 0;
}
