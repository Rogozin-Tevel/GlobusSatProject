#ifndef MAIN_H_
#define MAIN_H_


/*!
 * @brief checks if communication with the ground station was made in the last 'NO_COMMUNICATION_WDT_KICK_TIME' seconds
 * @return	TRUE if WDT was kicked and restart is needed
 * 			FALSE is WDT was NOT kicked and everything is fine. no restart needed
 */
Boolean GroundCommunicationWDTKick();

/*!
 * 	@brief The main operation function. This function implements the 'Loop'
 *  side of the logic and handles all of the satellite macro-logics(TRX,EPS etc')
 *  @note the function must use an infinite loop at its base;
 *  according to the logic and FreeRTOS thread requirements
 */
void taskmain();

/*!
 * This is the main function of the satellite operation
 * The main function contains a 'Boot' call which is operated once,
 * and the taskMain thread which is activated repeatedly.
 */
int main();

#endif /* MAIN_H_ */

