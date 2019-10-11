
#ifndef GLOBALSTANDARDS_H_
#define GLOBALSTANDARDS_H_

#include <hal/boolean.h>
#include <at91/utility/exithandler.h>
#include <hal/errors.h>

#include "FRAM_FlightParameters.h"
#include "SysI2CAddr.h"
#include "TestingConfigurations.h"
#ifndef NULL
	#define NULL ((void *)0)
#endif

#ifndef TRUE
	#define TRUE (0xFF)		//!< define logic 'true' value
#endif

#ifndef FALSE
	#define FALSE (0x00)	//!< define logic 'false' value
#endif



#define TELEMETRY_SAVE_TIME_PERIOD	(10)						///< save telemetry every 10 secondss

#define MINUTES_TO_MILLISECONDS(minute) ((minute)*60*1000)
#define SECONDS_TO_TICKS(secs) (portTickType)(((secs) * 1000) /portTICK_RATE_MS)
#define TICKS_TO_SECONDS(ticks) (unsigned int)(((ticks) / 1000) * portTICK_RATE_MS)
#define MINUTES_TO_TICKS(minutes)	(portTickType)(SECONDS_TO_TICKS(60*(minutes)))
#define MINUTES_TO_SECONDS(minutes)	(portTickType)(60*(minutes))

#define MAX_SEMAPHORE_WAIT (SECONDS_TO_TICKS(10))				///< wait for 10 seconds to acquire semaphore

#define DELAYED_CMD_TIME_BOUND	60 								///< if requested execution time has passed by this measure the command will not be executed and deleted from buffer

#define UNIX_DATE_JAN_D1_Y2000 {\
.seconds = 0,		\
.minutes = 0,		\
.hours = 0,			\
.day = 1,			\
.date = 1,			\
.month = 1,			\
.year = 0,			\
.secondsOfYear = 0} 					///< Time structure of the date 1/1/2000 at 00:00

#define UNIX_SECS_FROM_Y1970_TO_Y2000	946681200				///< number of seconds from 1 JAN 1970 to 1 JAN 2000

typedef unsigned long int time_unix; 	///< in seconds
typedef unsigned char byte;

typedef unsigned short voltage_t; 	///< in mV
typedef unsigned short current_t; 	///< in mA
typedef unsigned short power_t;		///< in mW
typedef short temp_t;				///< in Celsius
typedef unsigned char channel_t;	///< each bit represents a specific system state. 1 = ON; 0 = OFF


#endif /* GLOBALSTANDARDS_H_ */
