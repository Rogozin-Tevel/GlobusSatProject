#include "GlobalStandards.h"

/*
#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
*/
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif

#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <stdlib.h>
#include <string.h>

#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/PowerManagment/EPSOperationModes.h"
// #include "SubSystemModules/Communication/SatDataTx.h" // ???
#include "EPS_Commands.h"
#include <hal/errors.h>

// Shai (or someone else), check this pls?
int CMD_UpdateThresholdVoltages(sat_packet_t *cmd)
{ // Done by Maor
	if (cmd == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES];
	memcpy(thresh_volts, cmd->data, sizeof(thresh_volts));
	return UpdateThresholdVoltages(thresh_volts);
}

// Shai (or someone else), check this pls?
int CMD_GetThresholdVoltages(sat_packet_t *cmd)
{ // Done by Maor
	if (cmd == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES];
	int err = GetThresholdVoltages(thresh_volts);
	if (err)
	{
		return err;
	}

	err = TransmitDataAsSPL_Packet(cmd, thresh_volts, sizeof(thresh_volts));
	return err;
}

// Shai (or someone else), check this pls?
int CMD_RestoreDefaultAlpha(sat_packet_t *cmd)
{ // Done by Maor
	if (cmd == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	int err = RestoreDefaultAlpha();
	if (err)
	{
		return err;
	}

	return CMD_GetSmoothingFactor(cmd); // perhaps?
}

// Shai (or someone else), check this pls?
int CMD_RestoreDefaultThresholdVoltages(sat_packet_t *cmd)
{ // Done by Maor
	if (cmd == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	int err = RestoreDefaultThresholdVoltages();
	if (err)
	{
		return err;
	}

	return CMD_GetThresholdVoltages(cmd); // perhaps?
}

int CMD_UpdateSmoothingFactor(sat_packet_t* cmd)
{
	if (NULL == cmd)
	{
		return E_INPUT_POINTER_NULL;
	}

	float alpha;
	memcpy(&alpha, cmd->data, sizeof(alpha));
	return UpdateAlpha(alpha);
}

// Shai (or someone else), check this pls?
int CMD_GetSmoothingFactor(sat_packet_t* cmd)
{ // By Maor & Ido G
	if (cmd == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	// Fetch alpha by sending float pointer to method, and sending data as SPL packet by the trnsmitdata_As_SPL method
	float alpha;
	int err = GetAlpha(&alpha);
	if (err)
	{
		return err;
	}

	err = TransmitDataAsSPL_Packet(cmd, &alpha, sizeof(float));
	return err;
}

// Why is the packet necessary?

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
	(void)cmd;
	int err = 0;
#ifdef ISISEPS
	ieps_statcmd_t ieps_cmd;
	err = IsisEPS_noOperation(EPS_I2C_BUS_INDEX, &ieps_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif
	return err;
}

int CMD_EPS_ResetWDT(sat_packet_t *cmd)
{
	(void)cmd;
	int err = 0;
#ifdef ISISEPS
	ieps_statcmd_t ieps_cmd;
	err = IsisEPS_resetWDT(EPS_I2C_BUS_INDEX, &ieps_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif

	return err;
}

int CMD_EPS_SetChannels(sat_packet_t *cmd)
{
	if (NULL == cmd->data || NULL == cmd){
		return E_INPUT_POINTER_NULL;
	}
	int err = 0;
#ifdef ISISEPS
	ieps_statcmd_t ieps_cmd;
	ieps_obus_channel_t chmask3v3;
	ieps_obus_channel_t chmask5v;

	memcpy(&chmask3v3, cmd->data, sizeof(chmask3v3));
	memcpy(&chmask5v, cmd->data + sizeof(chmask3v3), sizeof(chmask5v));

	err = IsisEPS_outputBusGroupState(EPS_I2C_BUS_INDEX, chmask3v3, chmask5v,
			&ieps_cmd);
#endif
#ifdef GOMEPS
	//TODO: set Gomeps channels
#endif
	return err;
}

int CMD_SetChannels3V3_On(sat_packet_t *cmd)
{
	if (NULL == cmd->data || NULL == cmd){
		return E_INPUT_POINTER_NULL;
	}
	int err = 0;
#ifdef ISISEPS
	ieps_statcmd_t ieps_cmd;

	err = IsisEPS_outputBus3v3On(EPS_I2C_BUS_INDEX, 0, &ieps_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif

	return err;
}

int CMD_SetChannels3V3_Off(sat_packet_t *cmd)
{
	if (NULL == cmd->data)
	{
		return E_INPUT_POINTER_NULL;
	}
	int err = 0;
#ifdef ISISEPS
	ieps_statcmd_t ieps_cmd;

	err = IsisEPS_outputBus3v3Off(EPS_I2C_BUS_INDEX, 0, &ieps_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif

	return err;
}

int CMD_SetChannels5V_On(sat_packet_t *cmd)
{
	if (NULL == cmd->data || cmd == NULL){
		return E_INPUT_POINTER_NULL;
	}
	int err = 0;
#ifdef ISISEPS
	ieps_statcmd_t ieps_cmd;

	err = IsisEPS_outputBus5vOn(EPS_I2C_BUS_INDEX, 0, &ieps_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif

	return err;
}

int CMD_SetChannels5V_Off(sat_packet_t *cmd)
{
	if (NULL == cmd->data || NULL == cmd){
		return E_INPUT_POINTER_NULL;
	}
	int err = 0;

#ifdef ISISEPS
	ieps_statcmd_t ieps_cmd;

	err = IsisEPS_outputBus5vOff(EPS_I2C_BUS_INDEX, 0, &ieps_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif
	return err;
}

int CMD_GetEpsParameter(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL)
	{
		return E_INPUT_POINTER_NULL;
	}

	int err = 0;
#ifdef ISISEPS
	unsigned short int id = 0;
	memcpy(&id, cmd->data, sizeof(id));

	unsigned int par_size;
	err = IsisEPS_getParSize(id, &par_size);
	if (err != E_NO_SS_ERR){
		return err;
	}

	unsigned char *parameter = malloc(par_size);
	if (NULL == parameter){
		return E_MEM_ALLOC;
	}
	ieps_statcmd_t rsp_cmd;
	err = IsisEPS_getParameter(EPS_I2C_BUS_INDEX, id, (void*) parameter,
			&rsp_cmd);
	if (err == E_NO_SS_ERR){
		TransmitDataAsSPL_Packet(cmd, parameter, par_size);
	}
#endif
#ifdef GOMEPS
	//TODO:
#endif

	return err;
}

int CMD_SetEpsParemeter(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL){
		return E_INPUT_POINTER_NULL;
	}

	int err = 0;
#ifdef ISISEPS
	unsigned short int id = 0;
	memcpy(&id, cmd->data, sizeof(id));

	unsigned int par_size;
	err = IsisEPS_getParSize(id, &par_size);
	if (err != E_NO_SS_ERR){
		return err;
	}

	unsigned char *parameter = malloc(par_size);
	unsigned char *out_param = malloc(par_size);
	if (NULL == parameter || out_param == NULL){
		return E_MEM_ALLOC;
	}
	memcpy(parameter, cmd->data + sizeof(id), par_size);

	ieps_statcmd_t rsp_cmd;
	err = IsisEPS_setParameter(EPS_I2C_BUS_INDEX, id, (void*)parameter,(void*)out_param, &rsp_cmd);


	if (err == E_NO_SS_ERR){
		TransmitDataAsSPL_Packet(cmd, parameter, par_size);
	}
	free(parameter);
	free(out_param);
#endif
#ifdef GOMEPS
	//TODO:
#endif

	return err;
}

int CMD_ResetParameter(sat_packet_t *cmd)
{
	if (cmd == NULL || cmd->data == NULL){
		return E_INPUT_POINTER_NULL;
	}
	int err = 0;

#ifdef ISISEPS
	unsigned short int id = 0;
	memcpy(&id, cmd->data, sizeof(id));

	unsigned int par_size;
	err = IsisEPS_getParSize(id, &par_size);
	if (err != E_NO_SS_ERR){
		return err;
	}

	unsigned char *parameter = malloc(par_size);
	if (NULL == parameter){
		return E_MEM_ALLOC;
	}
	memcpy(parameter, cmd->data + sizeof(id), par_size);

	ieps_statcmd_t rsp_cmd;
	err = IsisEPS_resetParameter(ISIS_TRXVU_I2C_BUS_INDEX, id,
			(void*) parameter, &rsp_cmd);
	if (err == E_NO_SS_ERR){
		TransmitDataAsSPL_Packet(cmd, parameter, par_size);
	}
#endif
#ifdef GOMEPS
	//TODO:
#endif
	return err;
}

int CMD_ResetConfig(sat_packet_t *cmd)
{
	(void)cmd;
	int err = 0;
#ifdef ISISEPS
	ieps_statcmd_t rsp_cmd;
	err = IsisEPS_resetConfig(ISIS_TRXVU_I2C_BUS_INDEX, &rsp_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif
	return err;
}

int CMD_LoadConfig(sat_packet_t *cmd)
{
	(void)cmd;
	int err = 0;

#ifdef ISISEPS
	ieps_statcmd_t rsp_cmd;
	err = IsisEPS_loadConfig(ISIS_TRXVU_I2C_BUS_INDEX, &rsp_cmd);
#endif
#ifdef GOMEPS
	//TODO:
#endif
	return err;
}

int CMD_SaveConfig(sat_packet_t *cmd)
{
	(void)cmd;
	int err = 0;
	//TODO: CMD_SaveConfig, includeing CRC16- checksum and whatnot
	return err;
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
