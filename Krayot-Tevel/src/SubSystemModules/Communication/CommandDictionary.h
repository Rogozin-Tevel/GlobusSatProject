
#ifndef COMMANDDICTIONARY_H_
#define COMMANDDICTIONARY_H_

#include "SPL.h"
#include "SatCommandHandler.h"


/*!
 * @brief routes the data into the appropriate trxvu command according to the command sub type
 * @param[in] cmd command pertaining to the TRXVU system, to be executed.
 * @note the type and subtype of the command are already inside cmd
 * @see sat_packet_t structure
 * @return errors according to <hal/errors.h>
 */
int trxvu_command_router(sat_packet_t *cmd);

/*!
 * @brief routes the data into the appropriate eps command according to the command sub type
 * @param[in] cmd command pertaining to the EPS system, to be executed.
 * @note the type and subtype of the command are already inside cmd
 * @see sat_packet_t structure
 * @return errors according to <hal/errors.h>
 */
int eps_command_router(sat_packet_t *cmd);

/*!
 * @brief routes the data into the appropriate telemetry command according to the command sub type
 * @param[in] cmd command pertaining to the TLM sub routine, to be executed.
 * @note the type and subtype of the command are already inside cmd
 * @see sat_packet_t structure
 * @return errors according to <hal/errors.h>
 */
int telemetry_command_router(sat_packet_t *cmd);

/*!
 * @brief routes the data into the appropriate file-system command according to the command sub type
 * @param[in] cmd command pertaining to the FS system, to be executed.
 * @note the type and subtype of the command are already inside cmd
 * @see sat_packet_t structure
 * @return errors according to <hal/errors.h>
 */
int filesystem_command_router(sat_packet_t *cmd);

/*!
 * @brief routes the data into the appropriate obc command according to the command sub type
 * @param[in] cmd command pertaining to the MANAGMENT sub routine, to be executed.
 * @note the type and subtype of the command are already inside cmd
 * @see sat_packet_t structure
 * @return errors according to <hal/errors.h>
 */
int managment_command_router(sat_packet_t *cmd);

#endif /* COMMANDDICTIONARY_H_ */
