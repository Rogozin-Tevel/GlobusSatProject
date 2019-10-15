#include <satellite-subsystems/IsisTRXVU.h>
#include <hal/Timing/Time.h>
#include <string.h>
#include <stdlib.h>

#include "GlobalStandards.h"
#include "SatCommandHandler.h"


typedef struct __attribute__ ((__packed__)) delayed_cmd_t
{
	time_unix exec_time;	///< the execution time of the cmd in unix time
	sat_packet_t cmd;		///< command data
} delayed_cmd_t;

int ClearDelayedCMD_FromBuffer(unsigned int start_addr, unsigned int end_addr)
{
	return 0;
}

int ParseDataToCommand(unsigned char * data,sat_packet_t *cmd)
{


	if(NULL == data || NULL == cmd){
		return null_pointer_error;
	}
	void *err = NULL;

	unsigned int offset = 0;

	unsigned int id = 0;
	memcpy(&id,data,sizeof(id));
	if (NULL == err) {
			return execution_error;
	}
	offset += sizeof(id);

	char type;
	err = memcpy(&type,data+offset,sizeof(type));
	if (NULL == err) {
		return execution_error;
	}
	offset += sizeof(type);

	char subtype;
	err = memcpy(&subtype, data + offset,sizeof(subtype));
	if (NULL == err) {
		return execution_error;
	}
	offset += sizeof(subtype);


	unsigned int data_length = 0;
	err = memcpy(&data_length, data + offset,sizeof(data_length));
		if (NULL == err) {
			return execution_error;
		}
	offset += sizeof(data_length);

	return AssembleCommand(data+offset,data_length,type,subtype,id,cmd);

}

int AssmbleCommand(unsigned char *data, unsigned int data_length, char type,
		char subtype, unsigned int id, sat_packet_t *cmd)
{
	return 0;
}

// checks if a cmd time is valid for execution -> execution time has passed and command not expired
// @param[in] cmd_time command execution time to check
// @param[out] expired if command is expired the flag will be raised
Boolean isDelayedCommandDue(time_unix cmd_time, Boolean *expired)
{
	return FALSE;
}

//TOOD: move delayed cmd logic to the SD and write 'checked/uncheked' bits in the FRAM
int GetDelayedCommand(sat_packet_t *cmd)
{
	return 0;
}

int AddDelayedCommand(sat_packet_t *cmd)
{
	return 0;
}

int GetDelayedCommandBufferCount()
{
	return 0;

}

int GetOnlineCommand(sat_packet_t *cmd)
{
	return 0;
}

int GetDelayedCommandByIndex(unsigned int index, sat_packet_t *cmd)
{
	return 0;
}

int DeleteDelayedCommandByIndex(unsigned int index)
{
	return 0;
}

int DeleteDelayedBuffer()
{
	return 0;
}
