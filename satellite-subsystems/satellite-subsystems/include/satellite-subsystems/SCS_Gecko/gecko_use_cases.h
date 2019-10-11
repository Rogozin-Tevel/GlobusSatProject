/*
 * gecko_use_cases.h
 *
 *  Created on: 2 jan. 2018
 *      Author: pbot
 */

#ifndef INCLUDE_SATELLITE_SUBSYSTEMS_SCS_GECKO_GECKO_USE_CASES_H_
#define INCLUDE_SATELLITE_SUBSYSTEMS_SCS_GECKO_GECKO_USE_CASES_H_

#include <hal/boolean.h>
#include <stdint.h>

/**
 * @brief Higher level function for taking an image.
 * @details Internally this function executes the following steps:
 * 1. Set sensor PGA, ADC and sensor offset through register 0x0D.
 * 2. Set the exposure time in register 0x0E.
 * 3. Set the number of frames to capture register 0x06 (FRAME_AMOUNT)
 * 4. Set the frame rate to register 0x0B (FRAME_RATE)
 * 5. Switch to Imaging Mode (Sensor ON):
 *    - Note: Sensor settings (PGA, ADC, offset, exposure, frame rate, etc) is only applied when sensor is
 *    	turned ON. If settings are changed, sensor should be turned OFF and ON again.
 *    - In order to switch the sensor ON, set bit 0 of register 0x0A.
 *    - Read bit 17 of register 0x0C to verify sensor is ON and bit 18 of register 0x0C to verify sensor is
 *    	trained correctly.
 * 6. Image to Flash
 *    - Ensure Flash has been initialised. Flash initialisation done is indicated by 16 of 0x02.
 *    - Set the IMAGE_ID address to capture images to, register 0x05 with a 21-bit value.
 *    - Set sensor to capture image data (not test pattern) by clearing bit 4 of register 0x02.
 *    - Start sampling by setting bit 2 of register 0x02.
 *    - Imaging done is indicated by bit 25 of register 0x02.
 * @param adcGain - Digital gain to use when taking images
 * @param pgaGain - Analog gain to use when taking images
 * @param exposure - Exposure to use when taking images
 * @param frameAmount - Number of images to take
 * @param frameRate - Frame rate at which images should be taken
 * @param imageID - Index in memory where image capture should start
 * @return Use case result:
 *   0 = completed successfully
 *  -1 = could not turn off sensor
 *  -2 = could not set ADC gain
 *  -3 = could not set PGA gain
 *  -4 = could not set exposure
 *  -5 = could not set frame amount
 *  -6 = could not set frame rate
 *  -7 = could not turn on sensor
 *  -8 = sensor turn on timeout
 *  -9 = training timeout
 *  -10 = training error
 *  -11 = could not initialise flash
 *  -12 = could not set image ID
 *  -13 = could not disable test pattern
 *  -14 = could not start sampling
 *  -15 = sampling timeout
 *  -16 = could not clear sample flag
 *  -17 = could not turn of sensor
 */
int GECKO_UC_TakeImage( uint8_t adcGain, uint8_t pgaGain, uint32_t exposure, uint32_t frameAmount, uint32_t frameRate, uint32_t imageID );

/**
 * @brief Higher level function for reading an image.
 * @note An image is 136 pages long with every page having 4096 x 32-bit words
 * (136 x 4096 x 32 = 17,825,792, and size of an image: 2048 x 1088 x 8-bits = 17,825,792)
 * @details Internally this function executes the following steps:
 * 1. Ensure Flash has been initialised. Flash initialisation done is indicated by 16 of 0x02.
 * 2. Set IMAGE_ID to read from by writing a 12-bit address to register 0x05.
 * 3. Start read-out by setting bit 1 of register 0x02.
 * 4. Determine if data is ready for reading through monitoring bit 19 of register 0x02. Start SPI read-out
      when bit 19 is set.
 * 5. Monitor register 0x07 to see how many pages still need to be read for this frame and register 0x03 to
 *    see how many 32-bit SPI words still to read from this page.
 * 6. Read Image data from register 0x04, 32-bits at a time.
 * 7. Read-out complete is indicated by bit 24 of register 0x02.
 * @param imageID - ID of image to be read
 * @param buffer - buffer into which image should be read
 * @param fast - enable to skip word and page checking
 * @return Use case result:
 *   0 = completed successfully
 *  -1 = could not initialise flash
 *  -2 = could not set image ID
 *  -3 = could not start readout
 *  -4 = data read timeout
 *  -5 = word count mismatch during read
 *  -6 = page count mismatch during read
 *  -7 = read done flag not set
 *  -8 = could not clear read done flag
 */
int GECKO_UC_ReadImage( uint32_t imageID, uint32_t *buffer, Boolean fast );

/**
 * @brief Higher level function for erasing an image.
 * @details Internally this function executes the following steps:
 * 1. Set image ID to erase by writing a 12-bit address to register 0x05.
 * 2. Start erase process by setting bit 3 in register 0x02.
 * 3. Bit 26 of register 0x02 will indicate when IMAGE_ID has been erased
 * @param imageID - ID of image to erase
 * @return Use case result:
 *   0 = completed successfully
 *  -1 = could not set image ID
 *  -2 = could not start erase
 *  -3 = erase timeout
 *  -4 = could not clear erase done flag
 */
int GECKO_UC_EraseBlock( uint32_t imageID );

#endif /* INCLUDE_SATELLITE_SUBSYSTEMS_SCS_GECKO_GECKO_USE_CASES_H_ */
