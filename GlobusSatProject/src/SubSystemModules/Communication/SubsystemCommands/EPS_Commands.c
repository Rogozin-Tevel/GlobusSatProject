#include "GlobalStandards.h"

#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif


#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <stdlib.h>
#include <string.h>

#include  "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/PowerManagment/EPSOperationModes.h"
#include "EPS_Commands.h"
#include <hal/errors.h>
int CMD_UpdateThresholdVoltages(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetThresholdVoltages(sat_packet_t *cmd)
{
	return GetThresholdVoltages(/*voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES]*/);
}

int CMD_UpdateSmoothingFactor(sat_packet_t *cmd)
{
	return 0;
}

int CMD_RestoreDefaultAlpha(sat_packet_t *cmd)
{
	return 0;
}

int CMD_RestoreDefaultThresholdVoltages(sat_packet_t *cmd)
{
	return 0;
}

int CMD_UpdateSmoothingFactor(sat_packet_t* cmd)
{
	int err = 0;
	if (NULL == cmd)
	{
		return E_INPUT_POINTER_NULL;
	}
	float alpha;
	memcpy((unsigned char*)&alpha, cmd->data, sizeof(alpha));
	err = UpdateAlpha(alpha);
	return err;
}
int CMD_GetSmoothingFactor(sat_packet_t* cmd)
{
	// Fetch alpha by sending float pointer to method, and sending data as SPL packet by the trnsmitdata_As_SPL method

}
int CMD_EnterCruiseMode(sat_packet_t *cmd)
{
	return 0;
}

int CMD_EnterFullMode(sat_packet_t *cmd)
{
	return 0;
}

int CMD_EnterCriticalMode(sat_packet_t *cmd)
{
	return 0;
}

int CMD_EnterSafeMode(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetCurrentMode(sat_packet_t *cmd)
{
	return 0;
}

int CMD_EPS_NOP(sat_packet_t *cmd)
{
	return 0;
}

int CMD_EPS_ResetWDT(sat_packet_t *cmd)
{
	return 0;
}

int CMD_EPS_SetChannels(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SetChannels3V3_On(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SetChannels3V3_Off(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SetChannels5V_On(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SetChannels5V_Off(sat_packet_t *cmd)
{
	return 0;
}

int CMD_GetEpsParameter(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SetEpsParemeter(sat_packet_t *cmd)
{
	return 0;
}

int CMD_ResetParameter(sat_packet_t *cmd)
{
	return 0;
}

int CMD_ResetConfig(sat_packet_t *cmd)
{
	return 0;
}

int CMD_LoadConfig(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SaveConfig(sat_packet_t *cmd)
{
	return 0;
}

int CMD_SolarPanelWake(sat_packet_t* cmd)
{

	IsisSolarPanelv2_State_t state = 0;
	state = IsisSolarPanelv2_wakeup();
	TransmitDataAsSPL_Packet(cmd, &state, sizeof(state));
	return state;
}

int CMD_SolarPanelSleep(sat_packet_t* cmd)
{
	IsisSolarPanelv2_State_t state = 0;
	state = IsisSolarPanelv2_sleep();
	TransmitDataAsSPL_Packet(cmd, &state, sizeof(state));
	return state;
}

int CMD_GetSolarPanelState(sat_packet_t* cmd)
{
	IsisSolarPanelv2_State_t state = 0;
	state = IsisSolarPanelv2_getState();
	TransmitDataAsSPL_Packet(cmd, &state, sizeof(state));
	return state;
}
