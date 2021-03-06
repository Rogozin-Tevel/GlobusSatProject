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
		return null_pointer_error;
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
			return execution_error;
		}
	}
	return command_succsess;
}

// checks if a cmd time is valid for execution -> execution time has passed and command not expired
// @param[in] cmd_time command execution time to check
// @param[out] expired if command is expired the flag will be raised
Boolean isDelayedCommandDue(time_unix cmd_time, Boolean *expired)
{
	int err = 0;
	time_unix present_time = 0;
	err = Time_getUnixEpoch(&present_time);
	if (0 != err) {
		return FALSE;
	}
	if (NULL != expired) {
		*expired = (present_time - cmd_time) > DELAYED_CMD_TIME_BOUND;
	}

	if (cmd_time < present_time) {
		return TRUE;
	}
	return FALSE;
}

//TOOD: move delayed cmd logic to the SD and write 'checked/uncheked' bits in the FRAM
int GetDelayedCommand(sat_packet_t *cmd)
{
	if (NULL == cmd) {
		return cmd_null_pointer_error;
	}

	unsigned int current_fram_addr = DELAYED_CMD_BUFFER_ADDR;
	time_unix exec_time = 0;
	Boolean cmd_expired = FALSE;
	for (int i = 0; i < DELAYED_CMD_BUFFER_LENGTH; i++) {
		FRAM_read((unsigned char*) &exec_time, current_fram_addr,
				sizeof(time_unix));

		if (isDelayedCommandDue(exec_time, &cmd_expired)) {
			FRAM_read((unsigned char*) cmd,
					current_fram_addr + sizeof(time_unix),
					sizeof(delayed_cmd_t));
			ClearDelayedCMD_FromBuffer(current_fram_addr,
					current_fram_addr + sizeof(delayed_cmd_t));
			break;
		}
		else if (cmd_expired) {
			ClearDelayedCMD_FromBuffer(current_fram_addr,
					current_fram_addr + sizeof(delayed_cmd_t));
		}
		current_fram_addr += sizeof(delayed_cmd_t);
	}
	return cmd_command_found;
}

int AddDelayedCommand(sat_packet_t *cmd)
{
	if (NULL == cmd) {
		return cmd_null_pointer_error;
	}

	time_unix max_time = 0, temp_time = 0;
	unsigned int max_time_cmd_addr = 0;
	unsigned int offset = 0;
	for (unsigned int i = 0; i < MAX_NUM_OF_DELAYED_CMD; ++i) {
		offset = DELAYED_CMD_BUFFER_ADDR
				+ (sizeof(delayed_cmd_t)) * i;
		FRAM_read((unsigned char*) &temp_time, offset, sizeof(time_unix));
		if (0 == temp_time) {
			FRAM_write((unsigned char*) cmd, offset,
					sizeof(delayed_cmd_t));
			break;
		}
		if (temp_time > max_time) {
			max_time = temp_time;
			max_time_cmd_addr = offset;
		}
	}
	FRAM_write((unsigned char*) cmd, max_time_cmd_addr,
			sizeof(delayed_cmd_t));
	return cmd_command_success;
}

int GetDelayedCommandBufferCount()
{
	unsigned char frame_count = 0;
	int err = FRAM_read(&frame_count, DELAYED_CMD_FRAME_COUNT_ADDR,
	DELAYED_CMD_FRAME_COUNT_SIZE);
	return err ? -1 : frame_count;
}

int GetOnlineCommand(sat_packet_t *cmd)
{
	if (NULL == cmd) {
		return cmd_null_pointer_error;
	}
	int err = 0;

	unsigned short frame_count = 0;
	unsigned char received_frame_data[MAX_COMMAND_DATA_LENGTH];

	err = IsisTrxvu_rcGetFrameCount(0, &frame_count);
	if (0 != err) {
		return cmd_execution_error;
	}
	if (0 == frame_count) {
		return cmd_no_command_found;
	}
	ISIStrxvuRxFrame rxFrameCmd = { 0, 0, 0,
			(unsigned char*) received_frame_data }; // for getting raw data from Rx, nullify values

	err = IsisTrxvu_rcGetCommandFrame(0, &rxFrameCmd); //get the frame from the Rx buffer
	if (0 != err) {
		return cmd_execution_error;
	}

	err = ParseDataToCommand(received_frame_data,cmd);

	if (0 != err) {
		return cmd_execution_error;
	}
	return cmd_command_found;
}

int GetDelayedCommandByIndex(unsigned int index, sat_packet_t *cmd)
{
	if (NULL == cmd) {
		return cmd_null_pointer_error;
	}
	if (index > MAX_NUM_OF_DELAYED_CMD) {
		return cmd_index_out_of_bound;
	}
	if(0 != FRAM_read((unsigned char*) cmd,
			index * sizeof(delayed_cmd_t) + DELAYED_CMD_BUFFER_ADDR
					+ sizeof(time_unix), sizeof(delayed_cmd_t))){
		return cmd_execution_error;
	}
	return cmd_command_success;
}

int DeleteDelayedCommandByIndex(unsigned int index)
{
		int err = 0;
	if (index > MAX_NUM_OF_DELAYED_CMD) {
		return cmd_index_out_of_bound;
	}
	unsigned int end_addr = DELAYED_CMD_BUFFER_ADDR
			+ (index + 1) * sizeof(delayed_cmd_t);
	unsigned int start_addr = DELAYED_CMD_BUFFER_ADDR
			+ index * sizeof(delayed_cmd_t);
	err = ClearDelayedCMD_FromBuffer(start_addr, end_addr);
	return err;
}

int DeleteDelayedBuffer()
{
	unsigned int max_addr = DELAYED_CMD_BUFFER_ADDR
			+ MAX_NUM_OF_DELAYED_CMD * sizeof(delayed_cmd_t);
	unsigned int start_addr = DELAYED_CMD_BUFFER_ADDR;

	int err = ClearDelayedCMD_FromBuffer(start_addr, max_addr);
	return err;
}
