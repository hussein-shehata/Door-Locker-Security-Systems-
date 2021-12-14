/*
 * Lcd.h
 *
 *  Created on: Oct 7, 2021
 *      Author: Hussein
 */

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init();
/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command);
/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data);
/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char* str);
/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col);
 /*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen();
/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(uint32 data);
#endif /* LCD_H_ */
