
#include "EPSOperationModes.h"
#include "GlobalStandards.h"

#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif


//TODO: update functions to only the relevant channels
channel_t g_system_state;
EpsState_t state;
Boolean g_low_volt_flag = FALSE; // set to true if in low voltage

int EnterFullMode()
{
	return 0;
}

int EnterCruiseMode()
{
	return 0;
}

int EnterSafeMode()
{
	return 0;
}

int EnterCriticalMode()
{
	return 0;
}

int SetEPS_Channels(channel_t channel)
{
	return 0;
}

EpsState_t GetSystemState()
{
	return state;
}

channel_t GetSystemChannelState()
{
	return g_system_state;
}

Boolean EpsGetLowVoltageFlag()
{
	return FALSE;
}

void EpsSetLowVoltageFlag(Boolean low_volt_flag)
{
}

