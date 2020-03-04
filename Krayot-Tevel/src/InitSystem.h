/*
 *
 * @file	InitSystem.h
 * @brief	All crucial initialization functions in one place
 * @note	Order of function calls is important. Read system documents for further analysis
 */

#ifndef INITSYSTEM_H_
#define INITSYSTEM_H_

/*!
 * @brief	Starts the FRAM using drivers, and checks for errors.
 * @see FRAM.h
 */
int StartFRAM();

/*!
 * @brief	writes the default filght parameters to the corresponding FRAM addresses
 * @see FRAM_FlightParameters.h
 */
void WriteDefaultValuesToFRAM();

/*!
 * @brief	Starts the I2C using drivers, and checks for errors.
 * @see	I2C.h
 */
int StartI2C();


/*!
 * @brief	Starts the SPI using drivers, and checks for errors
 * @see	SPI.h
 */
int StartSPI();


/*!
 * @brief	Starts the Time module using drivers, and checks for errors.
 * @see Time.h
 */
int StartTIME();


/*!
 * @brief	executes all required initializations of systems, including sub-systems, and checks for errors
 * @return	0 successful init
 * 			-1 failed at init
 */
int InitSubsystems();

/*!
 * @brief	deployment procedure
 * @return	0 successful deployment
 * 			-1 failed to deploy
 */
int DeploySystem();

#endif /* INITSYSTEM_H_ */
