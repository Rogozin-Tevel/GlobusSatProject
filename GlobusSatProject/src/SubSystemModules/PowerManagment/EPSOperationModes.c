
#include "EPSOperationModes.h"
#include "GlobalStandards.h"

#ifdef ISISEPS
#include <satellite-subsystems/IsisEPS.h>
#endif // ISISEPS

#ifdef GOMEPS
#include <satellite-subsystems/GomEPS.h>
#endif // GOMEPS


//TODO: update functions to only the relevant channels
channel_t g_system_state;
EpsState_t state;
Boolean g_low_volt_flag = FALSE; // set to true if in low voltage

int EnterFullMode()
{
	state = FullMode;
	return 0;
}

int EnterCruiseMode()
{
	state = CruiseMode;
	return 0;
}

int EnterSafeMode()
{
	state = SafeMode;
	return 0;
}

int EnterCriticalMode()
{
	state = CriticalMode;
	return 0;
}

int SetEPS_Channels(channel_t channel)
{
	g_system_state = channel;
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
	return g_low_volt_flag;
}

void EpsSetLowVoltageFlag(Boolean low_volt_flag)
{
	g_low_volt_flag = low_volt_flag;
}

