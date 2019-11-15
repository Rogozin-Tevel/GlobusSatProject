#include <string.h>

#include "GlobalStandards.h"
#include "TRXVU.h"
#include "AckHandler.h"

int SendAckPacket(ack_subtype_t acksubtype, sat_packet_t *cmd,
		unsigned char *data, unsigned int length)  // Check needed...
{
	sat_packet_t packet;

	if (cmd == NULL)
	{
		packet.ID = 0; // Perhaps?
	}
	else
	{
		packet.ID = cmd->ID;
	}
	packet.cmd_type = ack_type;
	packet.cmd_subtype = acksubtype;
	packet.data = data;
	packet.length = length;

	return TransmitSplPacket(&packet, NULL);
}

void SendErrorMSG(ack_subtype_t fail_subt, ack_subtype_t succ_subt,
		sat_packet_t *cmd, int err)
{
	if (err == 0)
	{
		SendAckPacket(succ_subt, cmd, cmd->data, cmd->length); // Must be checked before used!!!!
	}
	else
	{
		SendAckPacket(fail_subt, cmd, cmd->data, cmd->length); // Must be checked before used!!!!
	}
}

void SendErrorMSG_IfError(ack_subtype_t subtype, sat_packet_t *cmd, int err)
{
	if (err == 0)
	{
		return;
	}

	SendErrorMSG(subtype, subtype, cmd, err);
}
