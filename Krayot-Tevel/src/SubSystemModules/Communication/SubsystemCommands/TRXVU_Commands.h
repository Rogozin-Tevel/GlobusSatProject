
#ifndef TRXVU_COMMANDS_H_
#define TRXVU_COMMANDS_H_

#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"

int CMD_StartDump(sat_packet_t *cmd);

int CMD_SendDumpAbortRequest(sat_packet_t *cmd);

int CMD_ForceDumpAbort(sat_packet_t *cmd);

int CMD_MuteTRXVU(sat_packet_t *cmd);

int CMD_UnMuteTRXVU(sat_packet_t *cmd);

int CMD_GetBaudRate(sat_packet_t *cmd);

int CMD_SetBaudRate(sat_packet_t *cmd);

int CMD_SetBeaconCycleTime(sat_packet_t *cmd);

int CMD_GetBeaconInterval(sat_packet_t *cmd);

int CMD_SetBeaconInterval(sat_packet_t *cmd);

int CMD_GetTxUptime(sat_packet_t *cmd);

int CMD_GetRxUptime(sat_packet_t *cmd);

int CMD_GetNumOfDelayedCommands(sat_packet_t *cmd);

int CMD_GetNumOfOnlineCommands(sat_packet_t *cmd);

int CMD_DeleteDelyedCmdByID(sat_packet_t *cmd);

int CMD_DeleteAllDelyedBuffer(sat_packet_t *cmd);

int CMD_AntSetArmStatus(sat_packet_t *cmd);

int CMD_AntGetArmStatus(sat_packet_t *cmd);

int CMD_AntGetUptime(sat_packet_t *cmd);

int CMD_AntCancelDeployment(sat_packet_t *cmd);
#endif
/*COMMANDS_H_ */
