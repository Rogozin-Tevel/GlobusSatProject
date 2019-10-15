/*
 * IsisEPS.h
 *
 *  Created on: 15 sep. 2015
 *      Author: malv & lrot
 */

#ifndef ISISEPS_H_
#define ISISEPS_H_

// Operational Commands
#define ISIS_EPS_I2CCMD_RESET						0xAA
#define	ISIS_EPS_I2CCMD_NOP							0x02
#define	ISIS_EPS_I2CCMD_CANCELOP					0x03
#define	ISIS_EPS_I2CCMD_WDT							0x04
#define	ISIS_EPS_I2CCMD_OUTBUSGR_ON					0x10
#define ISIS_EPS_I2CCMD_OUTBUSGR_OFF				0x11
#define ISIS_EPS_I2CCMD_OUTBUSGR_STATE				0x12
#define ISIS_EPS_I2CCMD_OUTBUS_3V3_ON				0x13
#define ISIS_EPS_I2CCMD_OUTBUS_3V3_OFF				0x14
#define ISIS_EPS_I2CCMD_OUTBUS_5V_ON				0x15
#define ISIS_EPS_I2CCMD_OUTBUS_5V_OFF				0x16
#define ISIS_EPS_I2CCMD_OUTBUS_HV_ON				0x19
#define ISIS_EPS_I2CCMD_OUTBUS_HV_OFF				0x1A

// Data Request Commands
#define ISIS_EPS_I2CCMD_GET_SYSTEMSTATE				0x41
#define ISIS_EPS_I2CCMD_GET_RAWHKDATA_MB			0x42
#define ISIS_EPS_I2CCMD_GET_ENGHKDATA_MB			0x43
#define ISIS_EPS_I2CCMD_GET_AVGHKDATA_MB			0x44
#define ISIS_EPS_I2CCMD_GET_RAWHKDATA_CDB			0x45
#define ISIS_EPS_I2CCMD_GET_ENGHKDATA_CDB			0x46
#define ISIS_EPS_I2CCMD_GET_AVGHKDATA_CDB			0x47
#define ISIS_EPS_I2CCMD_GET_OUTBUS_3V3_STATE		0x48
#define ISIS_EPS_I2CCMD_GET_OUTBUS_5V_STATE			0x49
#define ISIS_EPS_I2CCMD_GET_OUTBUS_BV_STATE			0x50
#define ISIS_EPS_I2CCMD_GET_OUTBUS_HV_STATE			0x51

// Configuration Commands
#define ISIS_EPS_I2CCMD_GET_PARAMETER				0x81
#define ISIS_EPS_I2CCMD_SET_PARAMETER				0x82
#define ISIS_EPS_I2CCMD_RESET_PARAMETER				0x83

#define ISIS_EPS_I2CCMD_RESET_CONF					0x86
#define ISIS_EPS_I2CCMD_LOAD_CONF					0x87
#define ISIS_EPS_I2CCMD_SAVE_CONF					0x88

// data sizes: hk data frame size MINUS 2 bytes (CC & STAT)!
#define IEPS_SYSTEMSTATE_SIZE 	  21
#define IEPS_HKDATAMB_SIZE        64
#define IEPS_HKDATACDB_SIZE       30
#define IEPS_LOSTATE3V3_SIZE      11			// 3V3 has 4 channels: 3 + 2 x 4 = 11
#define IEPS_LOSTATE5V_SIZE       11			// 5V has 4 channels: 3 + 2 x 4 = 11
#define IEPS_LOSTATEBV_SIZE       7				// BV has 2 channels: 3 + 2 x 2 = 7
#define IEPS_LOSTATEHV_SIZE       17			// HV has 7 channels: 3 + 7 x 2 = 17
#define IEPS_LOSTATE_MAX_SIZE     19			// this is actually the allocated size of the local struct: 3 + 2x8 = 19. This excludes the CC & STAT which is in a separate struct.

#define IEPS_HRESET_KEY			  0xA6			// hard reset key
#define IEPS_CONF_KEY		  	  0xA7			// load configuration key value required to be passed as a parameter to the load configuration function

/**
 * Enumeration of all IEPS modes
 */
typedef enum __attribute__ ((__packed__)) _ieps_mode_t
{
	ieps_mode_startup = 0, ///< startup mode
	ieps_mode_emlopo = 1, ///< emergency-low-power mode
	ieps_mode_nominal = 2 ///< nominal mode
} ieps_mode_t;

/**
 *  Enumeration of all IEPS parameters.
 */
typedef enum __attribute__ ((__packed__)) _ieps_gnlparam_t
{
    ieps_int8_val = 0x01, ///< The parameter is an integer type signed 8 bits value
    ieps_uint8_val = 0x02, ///< The parameter is an integer type unsigned 8 bits value
    ieps_int16_val = 0x03, ///< The parameter is an integer type signed 16 bits value
    ieps_uint16_val = 0x04, ///< The parameter is an integer type unsigned 16 bits value
    ieps_int32_val = 0x05, ///< The parameter is an integer type signed 32 bits value
    ieps_uint32_val = 0x06, ///< The parameter is an integer type unsigned 32 bits value
    ieps_float_val = 0x07, ///< The parameter is a float type value
    ieps_int64_val = 0x08, ///< The parameter is an integer type signed 8 bits value
    ieps_uint64_val = 0x09, ///< The parameter is an integer type signed 8 bits value
    ieps_double_val = 0x0A ///< The parameter is a double type value
} ieps_gnlparam_t;

/**
 *  Enumeration of all possible error codes.
 */
typedef enum __attribute__ ((__packed__)) _ieps_accepted_error_t
{
	ieps_cmd_accepted = 0x00, ///< Stat cmd error accepted
	ieps_cmd_rejected = 0x01, ///< Stat cmd error rejected
	ieps_cmd_rejected_invalid = 0x02, ///< Rejected: invalid command code
	ieps_cmd_rejected_parmiss = 0x03, ///< Rejected: parameter missing
	ieps_cmd_rejected_parinv = 0x04, ///< Rejected: parameter invalid
	ieps_cmd_rejected_ccunav = 0x05, ///< Rejected: CC unavailable in current mode
	ieps_cmd_reserved = 0x06, ///< Reserved value
	ieps_cmd_internal_error = 0x07 ///< Internal error occurred during processing
} ieps_accepted_error_t;

/**
 *  Enumeration of cause of last reset.
 */
typedef enum __attribute__ ((__packed__)) _ieps_reset_cause_t
{
	ieps_reset_cause_power_on = 0x00, ///< reset was caused by power-loss
	ieps_reset_cause_eps_watchdog = 0x01, ///< reset was caused by eps watchdog
	ieps_reset_cause_command = 0x02, ///< reset was commanded
	ieps_reset_cause_emlopo = 0x03 ///< reset was caused by emergency-low-power mode
} ieps_reset_cause_t;

/**
 *  Enumeration of iEPS board indices
 */
typedef enum __attribute__ ((__packed__)) _ieps_board_t
{
	ieps_board_mb = 0x00, ///< mainboard
	ieps_board_cdb1 = 0x01, ///< companion/daughterboard 1
	ieps_board_cdb2 = 0x02, ///< companion/daughterboard 2
	ieps_board_cdb3 = 0x03, ///< companion/daughterboard 3
	ieps_board_cdb4 = 0x04, ///< companion/daughterboard 4
	ieps_board_cdb5 = 0x05, ///< companion/daughterboard 5
	ieps_board_cdb6 = 0x06, ///< companion/daughterboard 6
	ieps_board_cdb7 = 0x07, ///< companion/daughterboard 7
} ieps_board_t;

/**
 *	Union consisting of bitflags used in the iEPS output bus channel status
 */
typedef union __attribute__ ((__packed__))  _ieps_obus_channel_t
{
	unsigned char raw; //!< Unformatted ISIS EPS output bus channel status
	struct __attribute__ ((__packed__))
	{
		unsigned char
		obus_ch0 : 1,
		obus_ch1 : 1,
		obus_ch2 : 1,
		obus_ch3 : 1,
		obus_ch4 : 1,
		obus_ch5 : 1,
		obus_ch6 : 1,
		obus_ch7 : 1;
	} fields; //!< Struct with individual fields of ISIS EPS output bus channel
} ieps_obus_channel_t;

/**
 *	bitflags used to indicate enabled state for iEPS items
 */
typedef union __attribute__ ((__packed__)) _ieps_item_state_t
{
	unsigned char raw; //!< Unformatted ISIS EPS item bitflags
	struct __attribute__ ((__packed__))
	{
		unsigned char
		item0 : 1,
		item1 : 1,
		item2 : 1,
		item3 : 1,
		item4 : 1,
		item5 : 1,
		item6 : 1,
		item7 : 1;
	} fields; //!< Struct with individual fields of ISIS EPS item bitflags
} ieps_item_state_t;

/**
 *  Struct for command default response structure.
 */
typedef union __attribute__ ((__packed__)) _ieps_statcmd_t
{
    /** Raw value **/
    unsigned char raw[2];
    struct __attribute__ ((__packed__))
    {
        unsigned char command_code; ///< iEPS response command code.
        unsigned char cmd_error : 4, ///< iEPS error command code.
        reserved_stat : 3, ///< iEPS reserved bits.
        new : 1; ///< iEPS new stat register flag.
    }fields;
} ieps_statcmd_t;

/**
 *	Union consisting of bitflags used in the board component availability identification
 */
typedef union __attribute__ ((__packed__)) _ieps_brd_comp_avail_t
{
	unsigned char raw; //!< Unformatted ISIS EPS board component
	struct __attribute__ ((__packed__))
	{
		unsigned char
		bus3V3 : 1,
		bus5V : 1,
		busBV : 1,
		busHV : 1,
		bat : 1,
		heater : 1,
		reserved : 1,
		mppt : 1;
	} fields; //!< Struct with individual fields of ISIS EPS  component availability
} ieps_brd_comp_avail_t;

/**
 *  Struct for iEPS current system state.
 */
typedef union __attribute__ ((__packed__)) _ieps_systemstate_t
{
    /** Raw value **/
    unsigned char raw[IEPS_SYSTEMSTATE_SIZE];
    struct __attribute__ ((__packed__))
    {
    	ieps_mode_t mode; ///< iEPS system state mode.
        unsigned char conf; ///< iEPS system state on board configuration has been changed since last config save.
        unsigned int uptime; ///< iEPS system state uptime.
        unsigned short error; ///< iEPS (first encountered) internal error value during control iteration.
        ieps_reset_cause_t reset_cause;	///< iEPS last reset cause
        unsigned short reset_cause_pwron_cnt;	///< iEPS reset cause power-on counter
        unsigned short reset_cause_wdg_cnt;	///< iEPS reset cause watchdog counter
        unsigned short reset_cause_cmd_cnt;	///< iEPS reset cause command counter
        unsigned short reset_cause_emlopo_cnt;	///< iEPS last reset cause emergency-low-power counter
        unsigned short reset_cause_mcu_subcnt;	///< mcu reset pwron subcounter; only incremented on pwr-on reset while power was available (i.e. due to MCU internal hw reset)
        unsigned short reset_cause_fic_subcnt;	///< fic reset subcounter; internal use
    }fields;
} ieps_systemstate_t;

/**
 *  Struct for iEPS mainboard housekeeping data in raw counts as extracted from the hardware. Conversions need to be performed when engineering values are needed.
 */
typedef union __attribute__ ((__packed__)) _ieps_rawhk_data_mb_t
{
    /** Raw value **/
    unsigned char raw[IEPS_HKDATAMB_SIZE];
    struct __attribute__ ((__packed__))
    {
    	ieps_board_t board;					///< index indicating which board the housekeeping data originated from; should always be 'ieps_board_mb'.
    	ieps_brd_comp_avail_t comp;			///< bitflag indicating which board components where successfully queried during the last iEPS data retrieval operation
    	ieps_obus_channel_t swion_3V3;		///< bitflag indicating which 3V3 output busses are commanded to the on state
    	ieps_obus_channel_t swion_5V;		///< bitflag indicating which 5V output busses are commanded to the on state
    	ieps_obus_channel_t swion_BV;		///< bitflag indicating which Battery Voltage output busses are commanded to the on state
    	ieps_obus_channel_t swion_HV;		///< bitflag indicating which High Voltage output busses are commanded to the on state
    	ieps_obus_channel_t pgood_3V3;		///< bitflag indicating which 3V3 output busses are above 90% of the target voltage
    	ieps_obus_channel_t pgood_5V;		///< bitflag indicating which 5V output busses are above 90% of the target voltage
    	ieps_obus_channel_t pgood_BV;		///< bitflag indicating which Battery Voltage output busses are above 90% of the target voltage
    	ieps_obus_channel_t pgood_HV;		///< bitflag indicating which High Voltage output busses are above 90% of the target voltage
    	ieps_item_state_t bat_charging;		///< bitflag indicating which batteries are charging (depends on amount of batteries present)
    	ieps_item_state_t bat_heating;		///< bitflag indicating which battery heaters are on (depends on amount of batteries present)

    	// note: the following 4 items are computed fields and are already in engineering values
    	unsigned short pwr_generating;		///< [mW]; total power being generated by the solar panels/mppts of the IEPS; this power is delivered to the internal IEPS power bus
    	unsigned short pwr_charging;		///< [mW]; total power exchanged with all batteries; this power is exchanged between battery and the internal IEPS power bus
    	unsigned short pwr_consuming;		///< [mW]; total power taken from the internal IEPS power bus
    	unsigned short pwr_delivering;		///< [mW]; total power provided to the satellite at the output of the IEPS

    	unsigned short obus3V3_curr_raw;	///< 3V3 output bus current for all channels combined in raw counts
    	unsigned short obus3V3_volt_raw;	///< 3V3 output bus voltage for all channels combined in raw counts
    	unsigned short obus5V_curr_raw;		///< 5V output bus current for all channels combined in raw counts
    	unsigned short obus5V_volt_raw;		///< 5V output bus voltage for all channels combined in raw counts
    	unsigned short obusBV_curr_raw;		///< BV output bus current for all channels combined in raw counts
    	unsigned short obusBV_volt_raw;		///< BV output bus voltage for all channels combined in raw counts
    	unsigned short chrg_bus_volt_raw;	///< externally applied charge bus voltage in raw counts
    	unsigned short pcb_temp_raw;		///< pcb temperature in raw counts
    	unsigned short mppt_temp_raw;		///< mppt temperature in raw counts
    	unsigned short mcu_temp_raw;		///< MCU temperature as measured by the internal MCU temperature sensor in raw counts
    	unsigned short mppt0_in_curr_raw;	///< first mppt input current in raw counts
    	unsigned short mppt0_in_volt_raw;	///< first mppt input voltage in raw counts
    	unsigned short mppt0_out_curr_raw;	///< first mppt output current in raw counts
    	unsigned short mppt0_out_volt_raw;	///< first mppt output voltage in raw counts
    	unsigned short mppt1_in_curr_raw;	///< second mppt input current in raw counts
    	unsigned short mppt1_in_volt_raw;	///< second mppt input voltage in raw counts
    	unsigned short mppt1_out_curr_raw;	///< second mppt output current in raw counts
    	unsigned short mppt1_out_volt_raw;	///< second mppt output voltage in raw counts
    	unsigned short mppt2_in_curr_raw;	///< third mppt input current in raw counts
    	unsigned short mppt2_in_volt_raw;	///< third mppt input voltage in raw counts
    	unsigned short mppt2_out_curr_raw;	///< third mppt output current in raw counts
    	unsigned short mppt2_out_volt_raw;	///< third mppt output voltage in raw counts
    }fields;
} ieps_rawhk_data_mb_t;

/**
 *  Struct for iEPS companion/daughterboard data in raw counts as extracted from the hardware. Conversions need to be performed when engineering values are needed.
 */
typedef union __attribute__ ((__packed__)) _ieps_rawhk_data_cdb_t
{
    /** Raw value **/
    unsigned char raw[IEPS_HKDATACDB_SIZE];
    struct __attribute__ ((__packed__))
    {
    	ieps_board_t board;					///< index indicating which board the housekeeping data originated from
    	ieps_brd_comp_avail_t comp;			///< bitflag indicating which board components where successfully queried during the last iEPS data retrieval operation
    	unsigned short obusHV_curr_raw;		///< high-voltage bus current in raw counts
    	unsigned short obusHV_volt_raw;		///< high-voltage bus coltage in raw counts
    	unsigned short bat_charge_raw; 		///< battery charge in raw counts
        unsigned short bat_current_raw; 	///< battery current in raw counts
        unsigned short bat_voltage_raw; 	///< battery voltage in raw counts
        unsigned short bat_temp_raw; 		///< battery temperature in raw counts
    	unsigned short mppt0_in_curr_raw;	///< first mppt input current in raw counts
    	unsigned short mppt0_in_volt_raw;	///< first mppt input voltage in raw counts
    	unsigned short mppt0_out_curr_raw;	///< first mppt output current in raw counts
    	unsigned short mppt0_out_volt_raw;	///< first mppt output voltage in raw counts
    	unsigned short mppt1_in_curr_raw;	///< second mppt input current in raw counts
    	unsigned short mppt1_in_volt_raw;	///< second mppt input voltage in raw counts
    	unsigned short mppt1_out_curr_raw;	///< second mppt output current in raw counts
    	unsigned short mppt1_out_volt_raw;	///< second mppt output voltage in raw counts
    }fields;
} ieps_rawhk_data_cdb_t;

/**
 *  Struct for iEPS mainboard housekeeping data in engineering values.
 */
typedef union __attribute__ ((__packed__)) _ieps_enghk_data_mb_t
{
    /** Raw value **/
    unsigned char raw[IEPS_HKDATAMB_SIZE];
    struct __attribute__ ((__packed__))
    {
    	ieps_board_t board;					///< index indicating which board the housekeeping data originated from; should always be 'ieps_board_mb'.
    	ieps_brd_comp_avail_t comp;			///< bitflag indicating which board components where successfully queried during the last iEPS data retrieval operation
    	ieps_obus_channel_t swion_3V3;		///< bitflag indicating which 3V3 output busses are commanded to the on state
    	ieps_obus_channel_t swion_5V;		///< bitflag indicating which 5V output busses are commanded to the on state
    	ieps_obus_channel_t swion_BV;		///< bitflag indicating which Battery Voltage output busses are commanded to the on state
    	ieps_obus_channel_t swion_HV;		///< bitflag indicating which High Voltage output busses are commanded to the on state
    	ieps_obus_channel_t pgood_3V3;		///< bitflag indicating which 3V3 output busses are above 90% of the target voltage
    	ieps_obus_channel_t pgood_5V;		///< bitflag indicating which 5V output busses are above 90% of the target voltage
    	ieps_obus_channel_t pgood_BV;		///< bitflag indicating which Battery Voltage output busses are above 90% of the target voltage
    	ieps_obus_channel_t pgood_HV;		///< bitflag indicating which High Voltage output busses are above 90% of the target voltage
    	ieps_item_state_t bat_charging;		///< bitflag indicating which batteries are charging (depends on amount of batteries present)
    	ieps_item_state_t bat_heating;		///< bitflag indicating which battery heaters are on (depends on amount of batteries present)
    	unsigned short pwr_generating;		///< total power being generated by the solar panels/mppts of the IEPS; this power is delivered to the internal IEPS power bus
    	signed short pwr_charging;			///< total power exchanged with all batteries (positive = absorbed by battery, negative = produced by battery); this power is exchanged between battery and the internal IEPS power bus
    	unsigned short pwr_consuming;		///< total power taken from the internal IEPS power bus
    	unsigned short pwr_delivering;		///< total power provided to the satellite at the output of the IEPS
    	unsigned short obus3V3_curr;		///< 3V3 output bus current for all channels combined in raw counts
    	unsigned short obus3V3_volt;		///< 3V3 output bus voltage for all channels combined in raw counts
    	unsigned short obus5V_curr;			///< 5V output bus current for all channels combined in raw counts
    	unsigned short obus5V_volt;			///< 5V output bus voltage for all channels combined in raw counts
    	unsigned short obusBV_curr;			///< BV output bus current for all channels combined in raw counts
    	unsigned short obusBV_volt;			///< BV output bus voltage for all channels combined in raw counts
    	unsigned short chrg_bus_volt;		///< externally applied charge bus voltage in engineering values
    	signed short pcb_temp;				///< pcb temperature in engineering values
    	signed short mppt_temp;				///< mppt temperature in engineering values
    	signed short mcu_temp;				///< MCU temperature as measured by the internal MCU temperature sensor in engineering values
    	unsigned short mppt0_in_curr;		///< first mppt input current engineering values
    	unsigned short mppt0_in_volt;		///< first mppt input voltage engineering values
    	unsigned short mppt0_out_curr;		///< first mppt output current engineering values
    	unsigned short mppt0_out_volt;		///< first mppt output voltage engineering values
    	unsigned short mppt1_in_curr;		///< second mppt input current engineering values
    	unsigned short mppt1_in_volt;		///< second mppt input voltage engineering values
    	unsigned short mppt1_out_curr;		///< second mppt output current engineering values
    	unsigned short mppt1_out_volt;		///< second mppt output voltage engineering values
    	unsigned short mppt2_in_curr;		///< third mppt input current engineering values
    	unsigned short mppt2_in_volt;		///< third mppt input voltage engineering values
    	unsigned short mppt2_out_curr;		///< third mppt output current engineering values
    	unsigned short mppt2_out_volt;		///< third mppt output voltage engineering values
    }fields;
} ieps_enghk_data_mb_t;

/**
 *  Struct for iEPS companion/daughterboard (cdb) housekeeping data in engineering values.
 */
typedef union __attribute__ ((__packed__)) _ieps_enghk_data_cdb_t
{
    /** Raw value **/
    unsigned char raw[IEPS_HKDATACDB_SIZE];
    struct __attribute__ ((__packed__))
    {
    	ieps_board_t board;					///< index indicating which board the housekeeping data originated from
    	ieps_brd_comp_avail_t comp;			///< bitflag indicating which board components where successfully queried during the last iEPS data retrieval operation
    	unsigned short obusHV_curr;			///< HV output bus current for all channels combined in engineering values
    	unsigned short obusHV_volt;			///< HV output bus voltage for all channels combined in engineering values
    	unsigned short bat_charge; 			///< battery charge in engineering values
        signed short bat_current; 			///< battery current in engineering values, positive flowing into the battery (charging), negative when flowing out of the battery (discharging)
        unsigned short bat_voltage; 		///< battery voltage in engineering values
        signed short bat_temp; 				///< battery temperature in engineering values
    	unsigned short mppt0_in_curr;		///< first mppt input current in engineering values
    	unsigned short mppt0_in_volt;		///< first mppt input voltage in engineering values
    	unsigned short mppt0_out_curr;		///< first mppt output current in engineering values
    	unsigned short mppt0_out_volt;		///< first mppt output voltage in engineering values
    	unsigned short mppt1_in_curr;		///< second mppt input current in engineering values
    	unsigned short mppt1_in_volt;		///< second mppt input voltage in engineering values
    	unsigned short mppt1_out_curr;		///< second mppt output current in engineering values
    	unsigned short mppt1_out_volt;		///< second mppt output voltage in engineering values
    }fields;
} ieps_enghk_data_cdb_t;

/**
 *  Struct for output bus latch-off state
 */
typedef union __attribute__ ((__packed__)) _ieps_lostate_t
{
    /** Raw value **/
    unsigned char raw[IEPS_LOSTATE_MAX_SIZE];
    struct __attribute__ ((__packed__))
    {
    	ieps_obus_channel_t swion;			///< bitflag output bus channel switch-is-on indication; 1 = bus channel is being commanded to 'on'
    	ieps_obus_channel_t pgood;			///< bitflag output bus channel power good indication; 1 = bus channel voltage 90% of target voltage at minimum
    	ieps_obus_channel_t los;			///< bitflag output bus channel latch-off state indication; 1 = bus channel latched-off due to overcurrent
    	unsigned short los_cnt[8];			///< amount of latch offs occurred over the units lifetime for each channel
    }fields;
} ieps_lostate_t;

/**
 *  @brief      Initialize the iEPS with the corresponding i2cAddress.
 *  @note       This function can only be called once.
 *  @param[in]  address array of iEPS I2C Address structure.
 *  @param[in]  number number of attached iEPS in the system to be initialized.
 *  @return     Error code according to <hal/errors.h>
 */
int IsisEPS_initialize(unsigned char *address, unsigned char number);

/**
 *  @brief       Hardware resets the iEPS device. This will cause a power cycle of the entire satellite if the EPS unit is the sole power provider to the satellite.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_hardReset(unsigned char index, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       No-operation command for iEPS device. Useful for determining EPS availability without issuing a 'real' command.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_noOperation(unsigned char index, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Cancel current operation in the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_cancelOperation(unsigned char index, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Resets the watchdog timer. Any other command also implicitly resets the watchdog timer.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_resetWDT(unsigned char index, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus group on for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   chmask3v3 mask for the 3v3 channels.
 *  @param[in]   chmask5v mask for the 5v channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBusGroupOn(unsigned char index, ieps_obus_channel_t chmask3v3, ieps_obus_channel_t chmask5v,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus group off for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   chmask3v3 mask for the 3v3 channels.
 *  @param[in]   chmask5v mask for the 5v channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBusGroupOff(unsigned char index, ieps_obus_channel_t chmask3v3, ieps_obus_channel_t chmask5v,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus group state for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   chmask3v3 mask for the 3v3 channels.
 *  @param[in]   chmask5v mask for the 5v channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBusGroupState(unsigned char index, ieps_obus_channel_t chmask3v3, ieps_obus_channel_t chmask5v,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus 3v3 channels on for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   ch3v3_index index of the 3v3 channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBus3v3On(unsigned char index, unsigned char ch3v3_index,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus 3v3 channels off for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   ch3v3_index index of the 3v3 channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBus3v3Off(unsigned char index, unsigned char ch3v3_index,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus 5v channels on for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   ch5v_index index of the 5v channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBus5vOn(unsigned char index, unsigned char ch5v_index,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus 5v channels off for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   ch5v_index index of the 5v channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBus5vOff(unsigned char index, unsigned char ch5v_index,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus Hv channels on for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   ch5v_index index of the Hv channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBusHvOn(unsigned char index, unsigned char chHv_index,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Output bus Hv channels off for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   ch5v_index index of the Hv channels.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_outputBusHvOff(unsigned char index, unsigned char chHv_index,  ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       System state data request for the iEPS device.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_sys_state structure with EPS system state information.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getSystemState(unsigned char index, ieps_systemstate_t* p_sys_state, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get housekeeping data for the mainboard in raw values.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_rawhk_data_mb structure with raw mainboard housekeeping data results.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getRawHKDataMB(unsigned char index, ieps_rawhk_data_mb_t* p_rawhk_data_mb, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get housekeeping data for the mainboard in engineering values.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_enghk_data_mb structure with engineering mainboard housekeeping data results.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getEngHKDataMB(unsigned char index, ieps_enghk_data_mb_t* p_enghk_data_mb, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get running-average housekeeping data for the mainboard in engineering values.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_enghk_data_mb structure with running-average engineering mainboard housekeeping data results.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getRAEngHKDataMB(unsigned char index, ieps_enghk_data_mb_t* p_raenghk_data_mb, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get housekeeping data for the companion/daughterboard (cdb) in raw values.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   board indicating the companion/daughterboard to get the data from.
 *  @param[out]  p_rawhk_data_cdb structure with raw companion/daughterboard housekeeping data results.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getRawHKDataCDB(unsigned char index, ieps_board_t board, ieps_rawhk_data_cdb_t* p_rawhk_data_cdb, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get housekeeping data for the companion/daughterboard (cdb) in engineering values.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   board indicating the companion/daughterboard to get the data from.
 *  @param[out]  p_enghk_data_cdb structure with engineering companion/daughterboard housekeeping data results.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getEngHKDataCDB(unsigned char index, ieps_board_t board, ieps_enghk_data_cdb_t* p_enghk_data_cdb, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get running-average housekeeping data for the companion/daughterboard (cdb) in engineering values.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   board indicating the companion/daughterboard to get the data from.
 *  @param[out]  p_enghk_data_cdb structure with running-average engineering companion/daughterboard housekeeping data results.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getRAEngHKDataCDB(unsigned char index, ieps_board_t board, ieps_enghk_data_cdb_t* p_raenghk_data_cdb, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get Output bus 3V3 channel latch-off state caused by overcurrent incidents.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_lo_state structure with latch-off state values and counters.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getLOState3V3(unsigned char index, ieps_lostate_t* p_lo_state, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get Output bus 5V channel latch-off state caused by overcurrent incidents.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_lo_state structure with latch-off state values and counters.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getLOState5V(unsigned char index, ieps_lostate_t* p_lo_state, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get Output bus BV channel latch-off state caused by overcurrent incidents.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_lo_state structure with latch-off state values and counters.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getLOStateBV(unsigned char index, ieps_lostate_t* p_lo_state, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get Output bus HV channel latch-off state caused by overcurrent incidents.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_lo_state structure with latch-off state values and counters.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getLOStateHV(unsigned char index, ieps_lostate_t* p_lo_state, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Get system parameter size in bytes.
 *  @param[in]   param_id parameter id.
 *  @param[out]  p_par_sz size of the parameter data in bytes, inferred from the param_id.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getParSize(unsigned short param_id, unsigned int* p_par_sz);

/**
 *  @brief       Get iEPS system parameters.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   param_id parameter id.
 *  @param[out]  vp_rsp_param parameter value.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_getParameter(unsigned char index, unsigned short param_id, void* vp_rsp_param, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Set iEPS system parameters.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   param_id parameter id.
 *  @param[in]   vp_param_input parameter value to set.
 *  @param[out]  vp_rsp_param new parameter value resulting from the operation. This could be different from the value supplied due to clamping to within the valid range by the IEPS.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_setParameter(unsigned char index, unsigned short param_id, void* vp_param_input, void* vp_rsp_param, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Reset iEPS system parameters.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   param_id parameter id.
 *  @param[out]  vp_rsp_param new parameter value resulting from the operation.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_resetParameter(unsigned char index, unsigned short param_id, void* vp_rsp_param, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Reset iEPS configuration to hard coded defaults.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_resetConfig(unsigned char index, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       (Re)load iEPS configuration from non-volatile memory.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_loadConfig(unsigned char index, ieps_statcmd_t* p_rsp_code);

/**
 *  @brief       Save iEPS configuration to non-volatile memory.
 *  @param[in]   index index of iEPS I2C bus address.
 *  @param[in]   checksum CRC16 checksum over the entire read/write parameter list contained in memory using the CRC-CCITT-BR polynomial (f(x)=x**16 + x**12 + x**5 + 1) using 0xFFFF as seed
 *  			 if the checksum does not match the save command is rejected and the configuration is not saved. In that case a load operation should be performed to recall
 *  			 the known configuration before re-trying a save.
 *  			 Provide the value 0 to force a save, the checksum check will not be performed in this case.
 *  @param[out]  p_rsp_code represents the default response for every command.
 *  @return      Error code according to <hal/errors.h>
 */
int IsisEPS_saveConfig(unsigned char index, unsigned short checksum, ieps_statcmd_t* p_rsp_code);

#endif /* ISISEPS_H_ */
