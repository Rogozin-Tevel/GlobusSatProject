/*
 * hooks.c
 *
 *  Created on: 16 jan. 2015
 *      Author: pbot
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <at91/utility/exithandler.h>

#include <stdio.h>

extern void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName) __attribute__ ((weak));
extern void vApplicationIdleHook( void ) __attribute__ ((weak));

/*
 * This hook is provided by FreeRTOS as a way for the upper layer (user code) to
 * handle a stack overflow without introducing an dependency in the FreeRTOS
 * code.
 */
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName)
{
	printf("\n\r STACK OVERFLOW DETECTED!! \n\r");
	printf(" Culprit task %p name: %s \n\r", pxTask, pcTaskName);
	printf(" !!Restarting Now!! \n\r");
	restart();
}

/*
 * This hook is provided by FreeRTOS as a way for the upper layer (user code) to
 * determine what happens to the iOBC when idle without introducing an
 * dependency in the FreeRTOS code. This is an ideal place to kick the watchdog.
 * It is imperative that the hook function does not call any API functions that
 * might cause the idle task to block (vTaskDelay(), or a queue or semaphore
 * function with a block time, for example).
 */
void vApplicationIdleHook( void )
{
	return;
}

