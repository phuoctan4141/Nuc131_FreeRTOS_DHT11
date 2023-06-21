/*
 * main.c
 * FreeRTOS Kernel V10.4.3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Created on: Jun 15, 2023
 * 		Author: OneTon
 */

#include <stdio.h>

/* Hardware and starter kit includes. */
#include "NUC131.h"
#include "Nuc131Config.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Sensor driver includes. */
#include "dht11.h"
#include "lcd12864.h"

static void prvSetupHardware( void );
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName );
void vApplicationTickHook( void );

/* onTaskCreate */
static void vtaskDHT11( void *pvParameters );
static void vtaskLCD12864( void *pvParameters );

/* onQueueHandle */
QueueHandle_t xQueue;

/* Global variable */
uint8_t sDATABUFF[16] = "Tran Phuoc Tan  ";

/*---------------------------------------------------------------------------------------------------------*/
/*  MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    /* Configure the hardware ready to run the test. */
    prvSetupHardware();

    printf("\n\nCPU @ %d Hz\n", SystemCoreClock);

    printf("FreeRTOS is creating... \n");
    xQueue = xQueueCreate( 5, sizeof(dht11_t) );
    xTaskCreate( vtaskDHT11, "DHT11", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
    xTaskCreate( vtaskLCD12864, "LCD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

    printf("FreeRTOS is starting... \n\n");
    vTaskStartScheduler();

    for( ;; );
}
/*-----------------------------------------------------------*/

static void vtaskDHT11( void *pvParameters )
{
	dht11_t dht;

    for( ;; )
    {
    	portENTER_CRITICAL();

    	 if (DHT11_Read( &dht ) == TRUE)
    	 {
    		 printf("Humidity: %d.%d, ", dht.g_u8Humidity_Int, dht.g_u8Humidity_Dec);
    		 printf("Temperature: %d.%d\n", dht.g_u8Temperature_Int, dht.g_u8Temperature_Dec);

    		 if( xQueueSend(xQueue, &dht, 0) != pdPASS )
    		 {
    			 printf("No queue space.\n");
    		 }

    	 }

    	portEXIT_CRITICAL();

    	vTaskDelay( 1500 / portTICK_PERIOD_MS );
    }
}
/*-----------------------------------------------------------*/

static void vtaskLCD12864( void *pvParameters )
{
	dht11_t c_dht, p_dht;

    for( ;; )
    {
    	portENTER_CRITICAL();

    	 if(xQueueReceive(xQueue, &c_dht, 0) == pdPASS)
    	 {
    		 if( DHT11_Compare( c_dht, p_dht ) == FALSE )
    		 {
    			 p_dht = c_dht;
        		 DHT11_Toa( c_dht , sDATABUFF );
        		 LCD12864_Clear_TextMode();
        		 LCD12864_PrintText(1, sDATABUFF);
    		 }
    	 }

    	portEXIT_CRITICAL();

    	vTaskDelay( 1500 / portTICK_PERIOD_MS );
    }

}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    /* Init LCD12864B */
    LCD12864_Init_GPIO();
	LCD12864_Init_TextMode();
	LCD12864_Clear_TextMode();
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()).  */
}
/*-----------------------------------------------------------*/

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
