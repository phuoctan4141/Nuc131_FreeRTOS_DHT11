/*
 * dht11.h
 *
 *  Created on: Jun 15, 2023
 *      Author: OneTon
 */

#ifndef DHT_DHT11_H_
#define DHT_DHT11_H_

#include <stdio.h>
#include "NUC131.h"

#define DHT11_Pin    	PA0
#define DHT11_Port   	PA
#define DHT11_PinMsk 	BIT0

#define DHT11_Max_Time 	40

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

typedef struct
{
	uint8_t g_u8Humidity_Int, g_u8Humidity_Dec;
	uint8_t g_u8Temperature_Int, g_u8Temperature_Dec;
} dht11_t ;

uint8_t DHT11_Reset( void );
uint8_t DHT11_Read( dht11_t *dht );

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* DHT_DHT11_H_ */











