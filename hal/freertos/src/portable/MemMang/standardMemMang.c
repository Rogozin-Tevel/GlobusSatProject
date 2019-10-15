/*
 * standardMemMang.c
 *
 *  Created on: 18-Oct-2012
 *      Author: Akhil Piplani
 */

#include "freertos/FreeRTOSConfig.h"
#include <stdlib.h>

void *pvPortMalloc( size_t xSize ) {
	return malloc(xSize);
}

void vPortFree( void *pv ) {
	free(pv);
}

void vPortInitialiseBlocks( void ) {

}

size_t xPortGetFreeHeapSize( void ) {
	return configTOTAL_HEAP_SIZE; // Dirty hack that always return the maximum configured heap-space as this is not available in the current standard library.
}
