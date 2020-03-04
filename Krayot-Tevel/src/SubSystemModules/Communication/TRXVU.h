#ifndef TRXVU_H_
#define TRXVU_H_

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <satellite-subsystems/IsisTRXVU.h>
#include "GlobalStandards.h"
#include "AckHandler.h"
#include "SatCommandHandler.h"

#define MAX_MUTE_TIME 	(5400) 	///< max mute duration will be 90 minutes = 60 *90 [sec]
#define MUTE_ON 		TRUE	///< mute is on flag
#define MUTE_OFF 		FALSE	///< mute is off flag

typedef struct __attribute__ ((__packed__))
{
	sat_packet_t *cmd;
	unsigned char dump_type;
	time_unix t_start;
	time_unix t_end;
} dump_arguments_t;


/*!
 * @brief initializes the TRXVU subsystem
 * @return	0 on successful init
 * 			errors according to <hal/errors.h>
 */
int InitTrxvu();

/*!
 * @brief The TRXVU logic according to the sub-system flowchart
 * @return	command_succsess on success
 * 			errors according to CMD_ERR enumeration
 * @see "SatCommandHandler.h"
 */
int TRX_Logic();

/*!
 * @brief checks if transmission is possible on grounds of low voltage and TX mute
 * @return	TRUE if transmission is allowed
 * 			FALSE if transmission is denied
 */
Boolean CheckTransmitionAllowed();

/*!
 * @brief 	Transmits a packet according to the SPL protocol
 * @param[in] packet packet to be transmitted
 * @param[out] avalFrames Number of the available slots in the transmission buffer of the VU_TC after the frame has been added. Set NULL to skip available slot count read-back.
 * @return    Error code according to <hal/errors.h>
 */
int TransmitSplPacket(sat_packet_t *packet, int *avalFrames);

/*!
 * @brief sends an abort message via a freeRTOS queue.
 */
void SendDumpAbortRequest();

/*!
 * @brief Closes a dump task if one is executing, using vTaskDelete.
 * @note Can be used to forcibly abort the task
 */
void AbortDump();

/*!
 * @brief dump telemetry to the ground station with telemetry in time range specified in 'cmd'
 * @param[in] cmd the dump command
 * @note this function starts a new dump task
 * @return	0 on success
 * 			-1 on failure
 */
int DumpTelemetry(sat_packet_t *cmd);

/*!
 * @brief transmits beacon according to beacon logic
 */
void BeaconLogic();

//TODO:document
int UpdateBeaconBaudCycle(unsigned char cycle);

//TODO:document
int UpdateBeaconInterval(time_unix intrvl);

/*!
 * @brief	mutes the TRXVU for a specified time frame
 * @param[in] duration for how long will the satellite be in mute state
 * @return	0 in successful
 * 			-1 in failure
 */
int muteTRXVU(time_unix duration);

/*!
 * @brief Cancels TRXVU mute - transmission is now enabled
 */
void UnMuteTRXVU();

/*!
 * @brief returns the current state of the mute flag
 * @return	MUTE_ON if mute is on
 * 			MUTE_OFF if mute is off
 */
Boolean GetMuteFlag();

/*!
 * @brief checks if the Trxvu mute time has terminated
 * @return	TRUE if the termination time has arrived
 * 			FALSE else
 */
Boolean CheckForMuteEnd();

/*!
 * @brief returns number of online frames are in the TRX frame buffer
 * @return	#number number of packets available
 * 			-1 in case of failure
 */
int GetNumberOfFramesInBuffer();

/*!
 * @brief returns the current baud rate of the TX
 * @param[out] bitrate the current baud rate of the satellite
 * @return errors according to <hal/errors.h>
 */
int GetTrxvuBitrate(ISIStrxvuBitrateStatus *bitrate);

/*!
 * @brief transmits data as SPL packet
 * @param[in] cmd the given command.
 * @param[in] data the outout data.
 * @param[in] length number of bytes in 'data' fields.
 * @return errors according to <hal/errors.h>
 */
int TransmitDataAsSPL_Packet(sat_packet_t *cmd, unsigned char *data, unsigned int length);

#endif
