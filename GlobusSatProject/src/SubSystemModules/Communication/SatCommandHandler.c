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
	if (end_addr <= start_addr || end_addr >= FRAM_MAX_ADDRESS) {
		return -1;
	}
	unsigned int region_size = (end_addr - start_addr) * sizeof(unsigned char);

	unsigned char* temp_arr = (unsigned char*)malloc(region_size);

	memset(temp_arr, 0, region_size);

	FRAM_write(temp_arr, start_addr, end_addr);

	free(temp_arr);

	unsigned char frame_count = 0;
	FRAM_read(&frame_count, DELAYED_CMD_FRAME_COUNT_ADDR,
		DELAYED_CMD_FRAME_COUNT_SIZE);

	frame_count--;

	FRAM_write(&frame_count, DELAYED_CMD_FRAME_COUNT_ADDR,
		DELAYED_CMD_FRAME_COUNT_SIZE);

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
	if (NULL == cmd) {
		return cmd_null_pointer_error;
	}
	cmd->ID = id;
	cmd->cmd_type = type;
	cmd->cmd_subtype = subtype;
	cmd->length = 0;

	if (NULL != data) {
		unsigned int size = (data_length > MAX_COMMAND_DATA_LENGTH) ?
			MAX_COMMAND_DATA_LENGTH : data_length;
		cmd->length = size;
		void* err = memcpy(cmd->data, data, data_length);

		if (NULL == err) {
			return cmd_execution_error;
		}
	}
	return cmd_command_succsess;
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
