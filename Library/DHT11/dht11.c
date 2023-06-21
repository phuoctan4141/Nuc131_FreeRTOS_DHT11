/*
 * dht11.c
 *
 *  Created on: Jun 15, 2023
 *      Author: OneTon
 */

#include "dht11.h"

uint8_t DHT11_Reset( void )
{
    uint32_t u32_poll_time;

    /* The Quasi-bidirection mode could be used as input with pull-up enable */
    GPIO_SetMode(DHT11_Port, DHT11_PinMsk, GPIO_PMD_QUASI);

    /* Send out start signal to DHT11 */
    /* Set data pin level from high to low */
    DHT11_Pin = 1;
    DHT11_Pin = 0;

    /* Take at least 18 ms to ensure DHT11's detection of signal */
    Delay_mS(18);
    /* Pull up data pin */
    DHT11_Pin = 1;

    /* Record current timer counter value */
    u32_poll_time = TIMER_GetCounter(TIMER0);

    /* Wait response from DHT11 by pulling down data pin */
    while (1)
    {
        if (DHT11_Pin == 0)
        {
            break;
        }

        if ((TIMER_GetCounter(TIMER0) - u32_poll_time) > 1000)
            /* DHT11 doesn't response over 1 ms */
            return FALSE;
    }

    /* Wait until DHT11 pulls up data pin level */
    while (1)
    {
        if (DHT11_Pin == 1)
        {
            break;
        }
    }

    /* Send start signal successfully */
    return TRUE;
}

uint8_t DHT11_Read( dht11_t *dht )
{
    uint32_t i;
    uint32_t u32_prev_time;
    uint8_t u8Checksum;

	uint8_t g_u8Humidity_Int, g_u8Humidity_Dec;
	uint8_t g_u8Temperature_Int, g_u8Temperature_Dec;

    /* Init TIMER0 to measure high level time */
	TIMER0_Init();

    /* Start communicate with DHT11 */
    if (DHT11_Reset() == TRUE)
    {
        /* Wait until DHT11 starts data transmission */
        while (DHT11_Pin == 1);

        /* A complete data transmission is 40 bits, DHT11 sends MSB first */
        /* 1st 8 bits is humidity integral data */
        g_u8Humidity_Int = 0;

        for (i = 0; i < 8; i++)
        {
            /* Shift current data for next bit */
            g_u8Humidity_Int = g_u8Humidity_Int << 1;

            /* Wait until DHT11 pull high data pin */
            while (DHT11_Pin == 0);

            /* Measure the high level time to determine the data bit */
            u32_prev_time = TIMER_GetCounter(TIMER0);

            while (DHT11_Pin == 1);

            if ((TIMER_GetCounter(TIMER0) - u32_prev_time) > DHT11_Max_Time)
            {
                g_u8Humidity_Int |= 0x01;
            }
        }

        /* 2nd 8 bits is humidity decimal data */
        g_u8Humidity_Dec = 0;

        for (i = 0; i < 8; i++)
        {
            /* Shift current data for next bit */
            g_u8Humidity_Dec = g_u8Humidity_Dec << 1;

            /* Wait until DHT11 pull high data pin */
            while (DHT11_Pin == 0);

            /* Measure the high level time to determine the data bit */
            u32_prev_time = TIMER_GetCounter(TIMER0);

            while (DHT11_Pin == 1);

            if ((TIMER_GetCounter(TIMER0) - u32_prev_time) > DHT11_Max_Time)
            {
                g_u8Humidity_Dec |= 0x01;
            }
        }

        /* 3rd 8 bits is temperature integral data */
        g_u8Temperature_Int = 0;

        for (i = 0; i < 8; i++)
        {
            /* Shift current data for next bit */
            g_u8Temperature_Int = g_u8Temperature_Int << 1;

            /* Wait until DHT11 pull high data pin */
            while (DHT11_Pin == 0);

            /* Measure the high level time to determine the data bit */
            u32_prev_time = TIMER_GetCounter(TIMER0);

            while (DHT11_Pin == 1);

            if ((TIMER_GetCounter(TIMER0) - u32_prev_time) > DHT11_Max_Time)
            {
                g_u8Temperature_Int |= 0x01;
            }
        }

        /* 4th 8 bits is temperature decimal data */
        g_u8Temperature_Dec = 0;

        for (i = 0; i < 8; i++)
        {
            /* Shift current data for next bit */
            g_u8Temperature_Dec = g_u8Temperature_Dec << 1;

            /* Wait until DHT11 pull high data pin */
            while (DHT11_Pin == 0);

            /* Measure the high level time to determine the data bit */
            u32_prev_time = TIMER_GetCounter(TIMER0);

            while (DHT11_Pin == 1);

            if ((TIMER_GetCounter(TIMER0) - u32_prev_time) > DHT11_Max_Time)
            {
                g_u8Temperature_Dec |= 0x01;
            }
        }

        /* 5th 8 bits is checksum */
        u8Checksum = 0;

        for (i = 0; i < 8; i++)
        {
            /* Shift current data for next bit */
            u8Checksum = u8Checksum << 1;

            /* Wait until DHT11 pull high data pin */
            while (DHT11_Pin == 0);

            /* Measure the high level time to determine the data bit */
            u32_prev_time = TIMER_GetCounter(TIMER0);

            while (DHT11_Pin == 1);

            if ((TIMER_GetCounter(TIMER0) - u32_prev_time) > DHT11_Max_Time)
            {
                u8Checksum |= 0x01;
            }
        }

        /* Stop TIMER0 */
        TIMER_Stop(TIMER0);

        /* Compare received data and checksum */
        if ((uint8_t)(g_u8Humidity_Int + g_u8Humidity_Dec + g_u8Temperature_Int + g_u8Temperature_Dec) == \
                u8Checksum)
        {
        	dht->g_u8Humidity_Int = g_u8Humidity_Int;
        	dht->g_u8Humidity_Dec = g_u8Humidity_Dec;
        	dht->g_u8Temperature_Int = g_u8Temperature_Int;
        	dht->g_u8Temperature_Dec = g_u8Temperature_Dec;

            return TRUE;
        }
        else
        {
            printf("Checksum error!\n");

            return FALSE;
        }
    }
    else
    {
        /* Stop TIMER0 */
        TIMER_Stop(TIMER0);

        printf("DHT11 is no response.\n");

        return FALSE;
    }
}

uint8_t DHT11_Compare( dht11_t c, dht11_t p)
{
	if( c.g_u8Humidity_Dec != p.g_u8Humidity_Dec ) return FALSE;
	if( c.g_u8Humidity_Int != p.g_u8Humidity_Int ) return FALSE;
	if( c.g_u8Temperature_Dec != p.g_u8Temperature_Dec ) return FALSE;
	if( c.g_u8Temperature_Int != p.g_u8Temperature_Int ) return FALSE;

	return TRUE;
}

void DHT11_Toa( dht11_t dht, uint8_t * sDATABUFF )
{
	uint8_t gDataBuff[16] = "Tran Phuoc Tan  ";
	uint8_t g_u8Humidity_Int[2], g_u8Humidity_Dec[2];
	uint8_t g_u8Temperature_Int[2], g_u8Temperature_Dec[2];

	utoa(dht.g_u8Humidity_Int, g_u8Humidity_Int, 10);
	utoa(dht.g_u8Humidity_Dec, g_u8Humidity_Dec, 10);

	utoa(dht.g_u8Temperature_Int, g_u8Temperature_Int, 10);
	utoa(dht.g_u8Temperature_Dec, g_u8Temperature_Dec, 10);

	gDataBuff[0] = 'H';
	gDataBuff[1] = ':';

	gDataBuff[2] = g_u8Humidity_Int[0];
	gDataBuff[3] = g_u8Humidity_Int[1];

	gDataBuff[4] = '.';

	gDataBuff[5] = g_u8Humidity_Dec[0];

	if( dht.g_u8Humidity_Dec >= 10) gDataBuff[6] = g_u8Humidity_Dec[1];
	else gDataBuff[6] = '0';

    gDataBuff[7] = ' ';
    gDataBuff[8] = ' ';

    gDataBuff[9] = 'T';
    gDataBuff[10] = ':';

    gDataBuff[11] = g_u8Temperature_Int[0];
    gDataBuff[12] = g_u8Temperature_Int[1];

    gDataBuff[13] = '.';

    gDataBuff[14] = g_u8Temperature_Dec[0];

    if( dht.g_u8Temperature_Dec >= 10 ) gDataBuff[15] = g_u8Temperature_Dec[1];
    else gDataBuff[15] = '0';

    uint8_t i;
    for(i = 0; i < 16 ; i++)
    {
    	sDATABUFF[i] = gDataBuff[i];
    }

    printf("sDATABUFF: %s \n", sDATABUFF);
}
