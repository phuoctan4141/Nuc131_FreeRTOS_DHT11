#ifndef NUC131_CONFIG_H
#define NUC131_CONFIG_H

#include "NUC131.h"

#define PLL_CLOCK   72000000

extern void SYS_Init( void )
{
	/*---------------------------------------------------------------------------------------------------------*/
	/* Init System Clock                                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/

	/* Enable Internal RC 22.1184MHz clock */
	CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

	/* Waiting for HIRC clock ready */
	CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

	/* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

	/* Waiting for external XTAL clock ready */
	CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

	/* Set core clock as PLL_CLOCK from PLL */
	CLK_SetCoreClock(PLL_CLOCK);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();

	/* Enable peripheral clock */
	CLK_EnableModuleClock(UART0_MODULE);
	CLK_EnableModuleClock(TMR0_MODULE);

	/* Peripheral clock source */
	CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_PLL, CLK_CLKDIV_UART(1));
	CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, 0);

	/*---------------------------------------------------------------------------------------------------------*/
	/* Init I/O Multi-function                                                                                 */
	/*---------------------------------------------------------------------------------------------------------*/

	/* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);
}

extern void UART0_Init( void )
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/

	/* Reset IP */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 115200);
}

extern void TIMER0_Init( void )
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init TIMER                                                                                              */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset TIMER0 */
    SYS_ResetModule(TMR0_RST);

    /* Set periodic mode */
    TIMER0->TCSR = TIMER_PERIODIC_MODE;

    /* Set prescaler value to make unit count time is 1 us */
    /* Timer clock input is HCLK = 72 MHz, prescaler value is HCLK/(10^6) - 1 = 71 */
    TIMER_SET_PRESCALE_VALUE(TIMER0, SystemCoreClock / 1000000 - 1);

    /* Set 24-bit comapre register value to maximum value 0xFFFFFF */
    TIMER_SET_CMP_VALUE(TIMER0, 0xFFFFFF);

    /* Enable TDR_EN to make TDR register be updated continuously to monitor 24-bit up counter value */
    TIMER0->TCSR |= TIMER_TCSR_TDR_EN_Msk;

    /* Start TIMER0 */
    TIMER_Start(TIMER0);
}

extern void GPIO_Init( void )
{
	GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
	PB13 = 1;
}

extern void Delay_mS(uint32_t m_second)
{
    uint32_t i;

    for (i = 0; i < m_second * 4 * 1000; i++)
    {
        asm("nop");
    }
}

#endif /* NUC131_CONFIG_H */
