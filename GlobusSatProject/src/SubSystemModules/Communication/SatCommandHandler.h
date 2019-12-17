#ifndef SATCOMMANDS_H_
#define SATCOMMANDS_H_

#define MAX_COMMAND_DATA_LENGTH 200 ///< maximum AX25 data field available for downlink

//<! how many command can be saved in the buffer
#define MAX_NUM_OF_DELAYED_CMD (100)


typedef enum __attribute__ ((__packed__)) CMD_ERR
{
	command_succsess = 0,				///< a successful operation. no errors
	command_found = 0,					///< a command was found
	no_command_found ,					///< no commands were found in command buffers
	index_out_of_bound,					///< index out of bound error
	null_pointer_error,					///< input parameter pointer is null
	execution_error 					///< an execution error has occured
}CMD_ERR;

typedef struct __attribute__ ((__packed__)) sat_packet_t
{
	unsigned int ID;
	char cmd_type;
	char cmd_subtype;
	unsigned int length;
	unsigned char data[MAX_COMMAND_DATA_LENGTH];

}sat_packet_t;

/*!
 * @brief parses given frame from TRXVU into 'sat_command_t' structure.
 * @param[in] data raw data from which to parse the SPL packet
 * @param[in] data_length length of data packet in bytes
 * @param[out] cmd pointer to parsed command buffer
 * @return	errors according to CMD_ERR
 */
int ParseDataToCommand(unsigned char * data, sat_packet_t *cmd);

/*!
 * @brief parses given frame from TRXVU into 'sat_command_t' structure.
 * @param[in] data data field of the SPL packet
 * @param[in] data_length length of data packet in bytes
 * @param[in] type command type
 * @param[in] subtype command subtype
 * @param[in] id the id of the specific command
 * @param[out] cmd pointer to parsed command buffer
 * @return	errors according to CMD_ERR
 */
int AssmbleCommand(unsigned char *data, unsigned int data_length, char type, char subtype,unsigned int id, sat_packet_t *cmd);

/*!
 * @brief returns a command to be executed if there is one in the delayed command buffer
 * @param[out] cmd pointer to parsed command
 * @return	errors according to CMD_ERR
 */
int GetDelayedCommand(sat_packet_t *cmd);

/*!
 * @brief sets the command to the first empty slot or the command with the farthest execution time
 * @param[in] cmd command to be added into the delated command buffer
 * @return	errors according to CMD_ERR
 */
int AddDelayedCommand(sat_packet_t *cmd);

/*!
 * @brief returns number of commands in the delayed command buffer
 * @return	#number number of delayed commands in buffer
 * @return	errors according to CMD_ERR
 */
int GetDelayedCommandBufferCount();

/*!
 * @brief returns an online(immediate) command to be executed if there is one in the command buffer
 * @param[out] cmd pointer to parsed command from online TRXVU frame buffer
 * @note cmd is set
 * @return	errors according to CMD_ERR
 */
int GetOnlineCommand(sat_packet_t *cmd);

//TODO: document
int GetDelayedCommandByIndex(unsigned int index, sat_packet_t *cmd);

int DeleteDelayedCommandByIndex(unsigned int index);

int DeleteDelayedBuffer();

#endif /* SATCOMMANDS_H_ */
