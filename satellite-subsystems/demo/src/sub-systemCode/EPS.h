/*
 * EPS.h
 *
 *  Created on: Oct 21, 2018
 *      Author: Hoopoe3n
 */

#ifndef EPS_H_
#define EPS_H_

#include <freertos/semphr.h>

#include <satellite-subsystems/GomEPS.h>

#include "Global/FRAMadress.h"
#include "Global/Global.h"
#include "Global/sizes.h"

#ifndef EPS_I2C_ADDR
#define EPS_I2C_ADDR 0x02
#endif

#define EPS_VOL_LOGIC_MAX 8100
#define EPS_VOL_LOGIC_MIN 6400

#define STATE_ADCS_ACT 0x01
#define STATE_TC 0x02
#define STATE_CAM 0x04
#define STATE_GS_MODE 0x08
#define STATE_5 0x10
#define STATE_6 0x20
#define STATE_7 0x40
#define STATE_8 0x80

/**
 * @brief Initializes the first platform consumption state
 */
void EPS_Init();

/**
 *TODO:
 */
void reset_FRAM_EPS();
void reset_EPS_voltages();

/**
 * @brief Does power conditioning and decides about satellite's consumption state
 */
void EPS_Conditioning();

Boolean8bit  get_shut_CAM();
void shut_CAM(Boolean mode);

Boolean8bit  get_shut_ADCS();
void shut_ADCS(Boolean mode);

/**
 * @brief Change switches according to full consumption mode.
 */
void EnterFullMode(gom_eps_channelstates_t *switches_states);

/**
 * @brief Change switches according to cruise consumption mode.
 */
void EnterCruiseMode(gom_eps_channelstates_t *switches_states);

/**
 * @brief Change switches according to safe consumption mode.
 */
void EnterSafeMode(gom_eps_channelstates_t *switches_states);

/**
 * @brief Change switches according to critical consumption mode.
 */
void EnterCriticalMode(gom_eps_channelstates_t *switches_states);

void WriteCurrentTelemetry(gom_eps_hk_t telemetry);

void convert_raw_voltage(byte raw[EPS_VOLTAGES_SIZE_RAW], voltage_t voltages[EPS_VOLTAGES_SIZE]);

#endif /* EPS_H_ */
