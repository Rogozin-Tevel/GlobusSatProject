/*
 * Main_Init.h
 *
 *  Created on: 15 2018
 *      Author: I7COMPUTER
 */

#ifndef MAIN_INIT_H_
#define MAIN_INIT_H_

int InitSubsystems();
/*!
 * @brief starts the tasks the flowing systems: trxvu , adcs ,payload controller
 */
int SubSystemTaskStart();

#endif /* MAIN_INIT_H_ */
