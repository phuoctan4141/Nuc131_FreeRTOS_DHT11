/*
 * lcd12864.c
 *
 *  Created on: Jun 20, 2023
 *      Author: OneTon
 */

#include "lcd12864.h"

void LCD_SPI_Write( uint8_t zdata )
{
	uint8_t i;
	for(i=0; i<8; i++)
	{
		if((zdata << i) & 0x80)
		{
			LCD_MOSI_Pin = 1; // MOSI = 1;
		}
		else
		{
			LCD_MOSI_Pin = 0; // MOSI = 0;
		}

		LCD_SCK_Pin = 0; //SCLK = 0;
		Delay_uS(10);
		LCD_SCK_Pin = 1; //SCLK = 1;

	}
}

void LCD_SPI_CS( uint8_t pol )
{
	if (pol!=0) LCD_SS_Pin = 1;
	else        LCD_SS_Pin = 0;
}

void LCD_WriteCommand( uint8_t cmd )
{
	LCD_SPI_CS(1);
	LCD_SPI_Write( 0xF8 );
	LCD_SPI_Write( 0xF0 & cmd );
	LCD_SPI_Write( 0xF0 & (cmd<<4) );
	LCD_SPI_CS(0);
}

void LCD_WriteData( uint8_t data )
{
	LCD_SPI_CS(1);
	LCD_SPI_Write( 0xFA );
	LCD_SPI_Write( 0xF0 & data );
	LCD_SPI_Write( 0xF0 & (data<<4) );
	LCD_SPI_CS(0);
}

void LCD12864_Init_GPIO( void )
{
	GPIO_SetMode(LCD_SS_Port, 	LCD_SS_PinMsk,	 GPIO_PMD_OUTPUT);
	GPIO_SetMode(LCD_MOSI_Port, LCD_MOSI_PinMsk, GPIO_PMD_OUTPUT);
	GPIO_SetMode(LCD_SCK_Port, 	LCD_SCK_PinMsk,  GPIO_PMD_OUTPUT);

	LCD_SS_Pin = 0;
	LCD_MOSI_Pin = 0;
	LCD_SCK_Pin = 0;
}

void LCD12864_Init_TextMode( void )
{
	LCD_WriteCommand(0x30);
	Delay_uS(100);
	LCD_WriteCommand(0x0C);
	Delay_uS(100);
}

void LCD12864_Clear_TextMode( void )
{
	LCD_WriteCommand(0x01);
	Delay_mS(100);
}

void LCD12864_PrintText(uint8_t line, uint8_t *string)
{
	uint8_t i, addr;
	if      (line==0) addr = 0x80;
	else if (line==1) addr = 0x90;
	else if (line==2) addr = 0x88;
	else if (line==3) addr = 0x98;
	else              addr = 0x80;
	LCD_WriteCommand(addr);

	for( i = 0; i < 16; i++ )
		LCD_WriteData( *string++ );
}
