/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * Stdio demo, upper layer of LCD driver.
 *
 * $Id: lcd.h 1008 2005-12-28 21:38:59Z joerg_wunsch $
 */

#ifndef __LCD_H__
#define __LCD_H__

/*
 * Initialize LCD controller.  Performs a software reset.
 */
void	lcd_init(void);


void lcd_putstring(char* String);

/*
 * Send one character to the LCD.
 */
int	lcd_putchar(char c);

void custom_character();


void lcd_pos(uint8_t , uint8_t);

void lcd_putint(int i);

void clear_screen(void);

#endif
