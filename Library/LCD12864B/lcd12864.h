/*
 * lcd12864.h
 *
 *  Created on: Jun 20, 2023
 *      Author: OneTon
 */

#ifndef LCD12864B_LCD12864_H_
#define LCD12864B_LCD12864_H_

#include <stdio.h>
#include "NUC131.h"

/**********************************/
/* RS-CS-DI -> SS -> PC.0 ->PB.11 */
/* R/W -> MOSI -> PC.3 ->PA.14    */
/* E -> SCK -> PC.1 ->PA.13       */
/**********************************/

#define LCD_SS_Pin			PB11
#define LCD_SS_Port			PB
#define LCD_SS_PinMsk		BIT11

#define LCD_MOSI_Pin		PA14
#define LCD_MOSI_Port		PA
#define LCD_MOSI_PinMsk		BIT14

#define LCD_SCK_Pin			PA13
#define LCD_SCK_Port		PA
#define LCD_SCK_PinMsk		BIT13

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

void LCD12864_Init_GPIO( void );
void LCD12864_Init_TextMode( void );
void LCD12864_Clear_TextMode( void );

void LCD12864_PrintText(uint8_t line, uint8_t *string);

 /* *INDENT-OFF* */
 #ifdef __cplusplus
 }
 #endif
 /* *INDENT-ON* */

#endif /* LCD12864B_LCD12864_H_ */
