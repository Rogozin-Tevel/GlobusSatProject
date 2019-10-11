
#ifndef ACKHANDLER_H_
#define ACKHANDLER_H_
#include <hal/errors.h>
#include "SatCommandHandler.h"
#include "SPL.h"


#define SendAnonymosAck(acksubtype) SendAckPacket(acksubtype,NULL,NULL,0) //<! sends an ACK without command ID nor data


/*!
 * @brief transmits an ack packet as an SPL packet(using the SPL protocol)
 * @param[in] acksubtype type of ack to be sent according to acl_subtype_t enumeration
 * @param[in] cmd the command for which the ACK is a response to. CAN BE NULL COMMAND FOR SYSTEM ACK!
 * @note the ACK is sent with the corresponding ID of the ACK inside the SPL packet
 * @return errors according t <hal/errors.h>
 */
int SendAckPacket(ack_subtype_t acksubtype, sat_packet_t *cmd, unsigned char *data, unsigned int length);

/*!
 * @brief transmits an ack packet as an SPL packet(using the SPL protocol)
 * @param[in] succ_subt type of ack to be sent according to acl_subtype_t enumeration when (err == 0)
 * @param[in] fail_subt type of ack to be sent according to acl_subtype_t enumeration when (err != 0)
 * @param[in] cmd the command for which the ACK is a response to.
 * @param[in] err the error msg to check by
 */
void SendErrorMSG( ack_subtype_t fail_subt, ack_subtype_t succ_subt, sat_packet_t *cmd, int err);

/*!
 * @brief transmits an ack packet as an SPL packet(using the SPL protocol) only if (err != 0)
 * @param[in] subtype type of ack to be sent according to acl_subtype_t enumeration when (err != 0)
 * @param[in] cmd the command for which the ACK is a response to.
 * @param[in] err the error msg to check by
 */
void SendErrorMSG_IfError(ack_subtype_t subtype, sat_packet_t *cmd, int err);

#endif /* ACKHANDLER_H_ */
