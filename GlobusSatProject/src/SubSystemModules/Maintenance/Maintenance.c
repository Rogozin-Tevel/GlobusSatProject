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

inline Boolean weak_assert(Boolean condition, const char *errMsg)
{
	if (!condition)
	{
		printf(errMsg);
	}
	return condition;
}

Boolean CheckExecutionTime(time_unix prev_time, time_unix period) // Fixed by Maor & Yanir
{
	time_unix current_time;

	weak_assert(Time_getUnixEpoch(&current_time) == 0,
			"Error in CheckExecutionTime: Can't load the current time!\n");

	return current_time - prev_time > period;
}

Boolean CheckExecTimeFromFRAM(unsigned int fram_time_addr, time_unix period) // Fixed by Maor & Yanir
{
	time_unix prev_time;

	weak_assert(FRAM_read((unsigned char*)&prev_time, fram_time_addr, sizeof(time_unix)) == 0,
			"Error in CheckExecTimeFromFRAM: Can't load the previous execution time from the FRAM!\n");

	return CheckExecutionTime(prev_time, period); // Return TRUE if the deadline has passed, FALSE otherwise.
}

void SaveSatTimeInFRAM(unsigned int time_addr, unsigned int time_size) // Fixed by Maor & Yanir
{
	time_unix current_time;

	weak_assert(Time_getUnixEpoch(&current_time) == 0,
			"Error in SaveSatTimeInFRAM: Can't load the current time!\n");

	weak_assert(FRAM_write((unsigned char *)&current_time, time_addr, time_size) == 0,
			"Error in SaveSatTimeInFRAM: Can't save the current time in the FRAM!\n");
}

Boolean IsFS_Corrupted() // Fixed by Maor & Yanir
{
	// Attempt to read from a random place in the memory.
	// If you succeed, the file system is OK, otherwise it's corrupted.

	// We randomly chose to read the first activation flag of the satellite, although it doesn't matter.
	unsigned char dummy_buffer[FIRST_ACTIVATION_FLAG_SIZE];

	// Return TRUE if the read has failed (so it's corrupted), FALSE otherwise.
	return FRAM_read(dummy_buffer, FIRST_ACTIVATION_FLAG_ADDR, FIRST_ACTIVATION_FLAG_SIZE) != 0;
}

int WakeupFromResetCMD() // Done by Blank, Fixed by Maor & Yanir
{
	time_unix current_time; // Current time received from function Time_getUnixEpoch

	*(RESET_CMD_FLAG_ADDR) = 0; // Lower the reset flag

	if (weak_assert(Time_getUnixEpoch(&current_time) != 1,
			"Error in WakeupFromResetCMD: Unix time isn't initialized!\n"))
	{
		return SendAckPacket(ACK_RESET_WAKEUP, NULL, (unsigned char *)&current_time, sizeof(time_unix));
	}
	return E_NOT_INITIALIZED;
}

void ResetGroundCommWDT() // Done by Blank
{
	WDT_forceKick(); // Force "kicking" the watchdog timer
}

Boolean IsGroundCommunicationWDTKick() // Fixed by Maor & Yanir
{
	time_unix current_time;
	time_unix last_communication_time;

	weak_assert(Time_getUnixEpoch(&current_time) == 0,
				"Error in IsGroundCommunicationWDTKick: Can't get the current time!\n");

	weak_assert(FRAM_read((unsigned char*)&last_communication_time, LAST_COMM_TIME_ADDR, LAST_COMM_TIME_SIZE) == 0,
			"Error in IsGroundCommunicationWDTKick: Can't load the last communication time from the FRAM!\n");

	// Return TRUE if the time passed since the last communication has exceeded the maximum time, FALSE otherwise.
	return current_time - last_communication_time >= DEFAULT_NO_COMM_WDT_KICK_TIME;
}

//TODO: add to command dictionary
int SetGsWdtKickTime(time_unix new_gs_wdt_kick_time) // Fixed by Maor & Yanir
{
	// TODO: Why do you need to check the new kick time? Why 1-2 days?
	if (new_gs_wdt_kick_time > SECONDS_IN_1_DAY * 2 || new_gs_wdt_kick_time < SECONDS_IN_1_DAY) // watch dog timer between one to two days
	{
		return -3; // TODO: Why return -3?
	}

	return FRAM_write((unsigned char *)&new_gs_wdt_kick_time, NO_COMM_WDT_KICK_TIME_ADDR, NO_COMM_WDT_KICK_TIME_SIZE);
}

time_unix GetGsWdtKickTime() // Fixed by Maor & Yanir
{
	time_unix gs_wdt_kick_time;

	weak_assert(FRAM_read((unsigned char *)&gs_wdt_kick_time, NO_COMM_WDT_KICK_TIME_ADDR, NO_COMM_WDT_KICK_TIME_SIZE) == 0,
			"Error in GetGsWdtKickTime: Can't load the ground station watch dog kick time from the FRAM!\n");

	return gs_wdt_kick_time;
}

void Maintenance()
{
	SaveSatTimeInFRAM(/*unsigned int time_addr, unsigned int time_size*/);

	WakeupFromResetCMD();  // Returns int

	IsFS_Corrupted();  // Returns boolean

	IsGroundCommunicationWDTKick();  // Returns boolean
}
