/*
 * gecko_driver.h
 *
 *  Created on: 20 dec. 2017
 *      Author: pbot
 */

#ifndef SRC_SCS_GECKO_GECKO_DRIVER_H_
#define SRC_SCS_GECKO_GECKO_DRIVER_H_

#include <hal/Drivers/SPI.h>

#include <stdint.h>

typedef enum GECKO_Endiannes_t
{
	GECKO_Endianness_big,
	GECKO_Endianness_little,
}
GECKO_Endianess_t;

/**
 *  @brief      Initialize the SCS Gecko driver with the corresponding SPI slave parameters.
 *  @param[in]  slaveParams SPIslaveParameters structure which should be used by the SPI driver calls.
 *  @return     0 = no error; -1 = error
 */
int GECKO_Init( SPIslaveParameters slaveParams );

/**
 *  @brief      Get a 32-bit register value from the Gecko subsystem.
 *  @param[in]  addr Address of registers to get.
 *  @param[out] data Data of the register to get.
 *  @param[in]  endian Endianess of how data should be read.
 *  @return     0 = no error; -1 = error, positive values defined in "SSerrors.h"
 */
int GECKO_GetReg( uint8_t addr, uint32_t *data, GECKO_Endianess_t endian );

/**
 *  @brief      Set a 32-bit register value on the Gecko subsystem.
 *  @param[in]  addr Address of registers to set.
 *  @param[in]  data Data of the register to set.
 *  @param[in]  endian Endianess of how data should be saved.
 *  @return     0 = no error; -1 = error, positive values defined in "SSerrors.h"
 */
int GECKO_SetReg( uint8_t addr, uint32_t data, GECKO_Endianess_t endian );

/**
 *  @brief Get the software build version from the version register.
 */
uint8_t GECKO_GetVersionSwBuild( void );

/**
 *  @brief Get the software version minor from the version register.
 */
uint8_t GECKO_GetVersionSwMinor( void );

/**
 *  @brief Get the software version major from the version register.
 */
uint8_t GECKO_GetVersionSwMajor( void );

/**
 *  @brief Get the hardware version from the version register.
 */
uint8_t GECKO_GetVersionHW( void );

/**
 *  @brief Start readout of data from the sensor.
 */
int GECKO_StartReadout( void );

/**
 *  @brief Start sampling of data on the sensor.
 */
int GECKO_StartSample( void );

/**
 *  @brief Start erasing of sensor data.
 */
int GECKO_StartErase( void );

/**
 *  @brief Enable test pattern.
 */
int GECKO_EnableTestPattern( void );

/**
 *  @brief Stop readout of data from the sensor.
 */
int GECKO_StopReadout( void );

/**
 *  @brief Stop sampling of data on the sensor.
 */
int GECKO_StopSample( void );

/**
 *  @brief Stop erasing of sensor data.
 */
int GECKO_StopErase( void );

/**
 *  @brief Disable test pattern.
 */
int GECKO_DisableTestPattern( void );

/**
 *  @brief Get flag indicating whether flash has been initialized.
 */
int GECKO_GetFlashInitDone( void );

/**
 *  @brief Get flag indicating whether data is ready to be read from sensor.
 */
int GECKO_GetReadReady( void );

/**
 *  @brief Get flag indicating whether data read is in progress.
 */
int GECKO_GetReadBusy( void );

/**
 *  @brief Get flag indicating whether sensor is busy sampling data.
 */
int GECKO_GetSampleBusy( void );

/**
 *  @brief Get flag indicating whether flash is being erased.
 */
int GECKO_GetEraseBusy( void );

/**
 *  @brief Get flag indicating whether data read from sensor is done.
 */
int GECKO_GetReadDone( void );

/**
 *  @brief Get flag indicating whether sensor is done sampling.
 */
int GECKO_GetSampleDone( void );

/**
 *  @brief Get flag indicating whether flash erase is done.
 */
int GECKO_GetEraseDone( void );

/**
 *  @brief Clear flag indicating whether data read from sensor is done.
 */
int GECKO_ClearReadDone( void );

/**
 *  @brief Clear flag indicating whether sensor is done sampling.
 */
int GECKO_ClearSampleDone( void );

/**
 *  @brief Clear flag indicating whether flash erase is done.
 */
int GECKO_ClearEraseDone( void );

/**
 *  @brief Return the number of 32-bit words left to read per page.
 */
uint16_t GECKO_GetFlashCount( void );

/**
 *  @brief Return 32-bit word of image data.
 */
uint32_t GECKO_GetImgData( void );

/**
 *  @brief Get the current image ID.
 */
uint32_t GECKO_GetImageID( void );

/**
 *  @brief Set the current image ID.
 */
int GECKO_SetImageID( uint32_t imageID );

/**
 *  @brief Get the number of frames to capture register.
 */
uint32_t GECKO_GetFrameAmount( void );

/**
 *  @brief Set the number of frames to capture register.
 */
int GECKO_SetFrameAmount( uint32_t frameAmount );

/**
 *  @brief Get the uptime of the subsystem.
 */
uint32_t GECKO_GetUpTime( void );

/**
 *  @brief Get the number of pages left to read per image frame.
 */
uint32_t GECKO_GetPageCount( void );

/**
 *  @brief Turn the sensor on.
 */
int GECKO_SensorOn( void );

/**
 *  @brief Turn the sensor off.
 */
int GECKO_SensorOff( void );

/**
 *  @brief Get the frame rate of the sensor.
 */
uint32_t GECKO_GetFrameRate( void );

/**
 *  @brief Set the frame rate of the sensor.
 */
int GECKO_SetFrameRate( uint32_t frameRate );

/**
 *  @brief Get flag indicating whether sensor is done turning on.
 */
int GECKO_GetOnDone( void );

/**
 *  @brief Get flag indicating whether sensor is done training.
 */
int GECKO_GetTrainingDone( void );

/**
 *  @brief Get flag indicating whether training had an error.
 */
int GECKO_GetTrainingError( void );

/**
 *  @brief Set the ADC gain of the sensor.
 */
int GECKO_SetAdcGain( uint8_t gain );

/**
 *  @brief Set the PGA gain of the sensor.
 */
int GECKO_SetPgaGain( uint8_t gain );

/**
 *  @brief Set the offset of the sensor.
 */
int GECKO_SetOffset( uint16_t offset );

/**
 *  @brief the exposure of the sensor.
 */
int GECKO_SetExposure( uint32_t exposure );

/**
 *  @brief Get input 5V voltage telemetry.
 */
float GECKO_GetVoltageInput5V( void );

/**
 *  @brief Get input 5V current telemetry.
 */
float GECKO_GetCurrentInput5V( void );

/**
 *  @brief Get FPGA 1V voltage telemetry.
 */
float GECKO_GetVoltageFPGA1V( void );

/**
 *  @brief Get FPGA 1V current telemetry.
 */
float GECKO_GetCurrentFPGA1V( void );

/**
 *  @brief Get FPGA 1V8 voltage telemetry.
 */
float GECKO_GetVoltageFPGA1V8( void );

/**
 *  @brief Get FPGA 1V8 current telemetry.
 */
float GECKO_GetCurrentFPGA1V8( void );

/**
 *  @brief Get FPGA 2V5 voltage telemetry.
 */
float GECKO_GetVoltageFPGA2V5( void );

/**
 *  @brief Get FPGA 2V5 current telemetry.
 */
float GECKO_GetCurrentFPGA2V5( void );

/**
 *  @brief Get FPGA 3V3 voltage telemetry.
 */
float GECKO_GetVoltageFPGA3V3( void );

/**
 *  @brief Get FPGA 3V3 current telemetry.
 */
float GECKO_GetCurrentFPGA3V3( void );

/**
 *  @brief Get Flash 1V8 voltage telemetry.
 */
float GECKO_GetVoltageFlash1V8( void );

/**
 *  @brief Get Flash 1V8 current telemetry.
 */
float GECKO_GetCurrentFlash1V8( void );

/**
 *  @brief Get Flash 3V3 voltage telemetry.
 */
float GECKO_GetVoltageFlash3V3( void );

/**
 *  @brief Get Flash 3V3 current telemetry.
 */
float GECKO_GetCurrentFlash3V3( void );

/**
 *  @brief Get sensor 1V8 voltage telemetry.
 */
float GECKO_GetVoltageSNSR1V8( void );

/**
 *  @brief Get sensor 1V8 current telemetry.
 */
float GECKO_GetCurrentSNSR1V8( void );

/**
 *  @brief Get sensor Vdd_pix voltage telemetry.
 */
float GECKO_GetVoltageSNSRVDDPIX( void );

/**
 *  @brief Get sensor Vdd_pix current telemetry.
 */
float GECKO_GetCurrentSNSRVDDPIX( void );

/**
 *  @brief Get sensor 3V3 voltage telemetry.
 */
float GECKO_GetVoltageSNSR3V3( void );

/**
 *  @brief Get sensor 3V3 current telemetry.
 */
float GECKO_GetCurrentSNSR3V3( void );

/**
 *  @brief Get Flash/RAM reference voltage telemetry.
 */
float GECKO_GetVoltageFlashVTT09( void );

/**
 *  @brief Get 3A and 3B SMPS temperature telemetry.
 */
float GECKO_GetTempSMU3AB( void );

/**
 *  @brief Get 3B and 3C SMPS temperature telemetry.
 */
float GECKO_GetTempSMU3BC( void );

/**
 *  @brief Get U6 regulator temperature telemetry.
 */
float GECKO_GetTempREGU6( void );

/**
 *  @brief Get U8 regulator temperature telemetry.
 */
float GECKO_GetTempREGU8( void );

/**
 *  @brief Get flash temperature telemetry.
 */
float GECKO_GetTempFlash( void );

#endif /* SRC_SCS_GECKO_GECKO_DRIVER_H_ */
