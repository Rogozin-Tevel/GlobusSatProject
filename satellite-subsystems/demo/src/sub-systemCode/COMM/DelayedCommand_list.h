/*
 * DelayedCommand_list.h
 *
 *  Created on: Apr 6, 2019
 *      Author: Hoopoe3n
 */

#ifndef DELAYEDCOMMAND_LIST_H_
#define DELAYEDCOMMAND_LIST_H_

#include "../Global/Global.h"
#include "GSC.h"
#include "../Global/sizes.h"

#define NOT_DELAYED_COMMAND 0
#define EXPIRED_TIME_DC (15 * 60)
#define MAX_NUMBER_OF_DELAY_COMMAND			100 //the max number of delayed commands in the FRAM


/**
 * 	@brief		goes over the delayed command list in the RAM and checks if time to execute one of the commands.
 * 				If the list changed the new list is place in the FRAM
 * 				deleting expired commands
 */
void check_delaycommand();

/**
 * 	@brief		reset the delayed command list in the FRAM, deleting all commands
 */
void reset_delayCommand(Boolean firstActivation);

/**
 *  @brief       add new command to the delayed command list in an empty space
 *  			 If there's no free place on the list the function delete one command with EmptyOldestCommand()
 *  @param[in]   the command to add to the delayed command list
 *  @return      0 if everything work, 1 if the list is full, -1 if the command is corrupted
 */
int add_delayCommand(TC_spl decode);

/*
 * @brief	Read from the FRAM the current list to a ram buffer
 */
void get_delayCommand_list();

/*
 * @brief	Delete the command with the latest time stamp to execute
 */
void EmptyOldestCommand();
#endif /* DELAYEDCOMMAND_LIST_H_ */
