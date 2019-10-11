#ifndef EPSOPERATIONMODES_H_
#define EPSOPERATIONMODES_H_


#include "GlobalStandards.h"

//TODO: change 'SYSTEMX' to the actual system
#define CHANNELS_OFF 0x00 	//!< channel state when all systems are off
#define CHNNELS_ON	 0XFF	//!< channel
#define SYSTEM0		 0x01	//!< channel state when 'SYSTEM0' is on
#define SYSTEM1		 0x02	//!< channel state when 'SYSTEM1' is on
#define SYSTEM2 	 0x04	//!< channel state when 'SYSTEM2' is on
#define SYSTEM3		 0x08	//!< channel state when 'SYSTEM3' is on
#define SYSTEM4		 0x10	//!< channel state when 'SYSTEM4' is on
#define SYSTEM5		 0x20	//!< channel state when 'SYSTEM5' is on
#define SYSTEM6 	 0x40	//!< channel state when 'SYSTEM6' is on
#define SYSTEM7 	 0x80	//!< channel state when 'SYSTEM7' is on

typedef enum{
	FullMode,
	CruiseMode,
	SafeMode,
	CriticalMode
}EpsState_t;

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Full mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterFullMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Cruise mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCruiseMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Safe mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterSafeMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Critical mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCriticalMode();

/*!
 * @brief Sets the channel state according to the bitwise 'logic on'
 * if the 2'nd bit is '1' the second channel will turn on (channel = 0b01000000)
 * @note please use the defines defined in this header to turn on/off channels
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int SetEPS_Channels(channel_t channel);

/*!
 * returns the current system state according to the EpsState_t enumeration
 * @return system state according to EpsState_t
 */
EpsState_t GetSystemState();

/*
 * Gets the current system channel state
 * @return current system channel state
 */
channel_t GetSystemChannelState();

Boolean EpsGetLowVoltageFlag();

void EpsSetLowVoltageFlag(Boolean low_volt_flag);


#endif /* EPSOPERATIONMODES_H_ */
