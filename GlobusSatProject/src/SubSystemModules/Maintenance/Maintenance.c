#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Timing/Time.h>

#include "GlobalStandards.h"

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>

#include <hcc/api_fat.h>

#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "TLM_management.h"
#include "Maintenance.h"

#include "FRAM_FlightParameters.h"

#define SECONDS_IN_1_DAY (60 * 60 * 24)

// Maor 7.12.2019:
// The error "handling" might be redundant, so I have removed it out meanwhile.

Boolean CheckExecutionTime(time_unix prev_time, time_unix period)
{
	time_unix current_time;
	Time_getUnixEpoch(&current_time);
	return current_time - prev_time > period;
}

Boolean CheckExecTimeFromFRAM(unsigned int fram_time_addr, time_unix period)
{
	time_unix prev_time;
	FRAM_read(&prev_time, fram_time_addr, sizeof(time_unix));
	return CheckExecutionTime(prev_time, period);
}

void SaveSatTimeInFRAM(unsigned int time_addr, unsigned int time_size)
{
	time_unix current_time;
	Time_getUnixEpoch(&current_time);
	FRAM_write(&current_time, time_addr, time_size);
}

Boolean IsFS_Corrupted()
{
	/*
		Attempt to read from a random place in the memory.
		If you succeed, the file system is OK, otherwise it's corrupted.
	*/

	// We randomly chose to read a random property that's stored in the FRAM.
	unsigned char dummy_buffer[RESET_CMD_FLAG_SIZE];

	// Return TRUE if the read has failed (so it's corrupted), FALSE otherwise.
	return FRAM_read(dummy_buffer, RESET_CMD_FLAG_ADDR, RESET_CMD_FLAG_SIZE);
}

// TODO: Check this, seems pretty damn short
void ResetGroundCommWDT() // Done by Blank
{
	WDT_forceKick(); // Force "kicking" the watchdog timer
}

Boolean IsGroundCommunicationWDTKick()
{
	time_unix current_time;
	Time_getUnixEpoch(&current_time);

	time_unix last_communication_time;
	FRAM_read(&last_communication_time, LAST_COMM_TIME_ADDR, LAST_COMM_TIME_SIZE);

	// Return TRUE if the time passed since the last communication has exceeded the maximum time, FALSE otherwise.
	return current_time - last_communication_time >= DEFAULT_NO_COMM_WDT_KICK_TIME;
}

int SetGsWdtKickTime(time_unix new_gs_wdt_kick_time)
{
	return FRAM_write(&new_gs_wdt_kick_time, NO_COMM_WDT_KICK_TIME_ADDR, NO_COMM_WDT_KICK_TIME_SIZE);
}

time_unix GetGsWdtKickTime()
{
	time_unix gs_wdt_kick_time;
	FRAM_read(&gs_wdt_kick_time, NO_COMM_WDT_KICK_TIME_ADDR, NO_COMM_WDT_KICK_TIME_SIZE);
	return gs_wdt_kick_time;
}

int WakeupFromResetCMD()
{
	// Lower the reset flag
	unsigned char data[1] = { 0 }; // The flag is either 0 or 1.
	int err = FRAM_write(data, RESET_CMD_FLAG_ADDR, RESET_CMD_FLAG_SIZE);
	if (err) 
	{
		printf("Error in WakeupFromResetCMD: Can't lower the reset flag!\n");
		return err; // Not sure what error code from hal/errors.h is suitable here.
	}

	// Load the current time
	time_unix current_time;
	err = Time_getUnixEpoch(&current_time);
	if (err)
	{
		printf("Error in WakeupFromResetCMD: Unix time isn't initialized!\n");
		return E_NOT_INITIALIZED;
	}

	return SendAckPacket(ACK_RESET_WAKEUP, NULL, &current_time, sizeof(time_unix));
}

// TODO: Do this!
void Maintenance()
{
	// SaveSatTimeInFRAM(/*unsigned int time_addr, unsigned int time_size*/);

	// WakeupFromResetCMD();  // Returns int

	// IsFS_Corrupted();  // Returns boolean

	// IsGroundCommunicationWDTKick();  // Returns boolean
}
