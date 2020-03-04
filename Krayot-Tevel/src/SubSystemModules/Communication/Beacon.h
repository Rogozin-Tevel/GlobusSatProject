#ifndef BEACON_H_
#define BEACON_H_

/*!
 *	@brief initializes the relevant parameters for
 *	the beacon module ongoing work
 */
void InitBeaconParams();

/*!
 * @brief transmits beacon according to beacon logic
 */
void BeaconLogic();

/*!
 * @brief Sets the cycle paramater in the FRAM if the value is within bounds.
 * @param[in] cycle after 'cycle' beacons the beacon transmission will be in 1200bps
 * @return	Errors according to <hal/errors.h>
 * 			E_PARAM_OUTOFBOUNDS if 'cycle' is not legal(too big\too small)
 * @note for example every 3'rd beacon will be in 1200bps ->
 * 			cycle = 3 -> bps = 1200 -> bps = 9600 -> bps = 9600 -> bps = 1200.
 *
 * 			minimum cycle is DEFALUT_BEACON_BITRATE_CYCLE
 */
int UpdateBeaconBaudCycle(unsigned char cycle);

/*!
 * @brief updates the time period between two beacons.
 * @param[in] intrvl the time to be set
 * @return	Errors according to <hal/errors.h>
 * 			E_PARAM_OUTOFBOUNDS if 'intrvl' is not legal(too big\too small)
 * @note updates in the FRAM as well as the private global variable.
 */
int UpdateBeaconInterval(time_unix intrvl);


#endif /* BEACON_H_*/
