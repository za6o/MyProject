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
 * $Id: lcd.c 2265 2011-12-28 21:28:29Z joerg_wunsch $
 */

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>

#include <util/delay.h>

#include "hd44780.h"
#include "lcd.h"

/*
 * Setup the LCD controller.  First, call the hardware initialization
 * function, then adjust the display attributes we want.
 */
void lcd_init(void)
{

  hd44780_init();

  /*
   * Clear the display.
   */
  hd44780_outcmd(HD44780_CLR);
  hd44780_wait_ready(true);

  /*
   * Entry mode: auto-increment address counter, no display shift in
   * effect.
   */
  hd44780_outcmd(HD44780_ENTMODE(1, 0));
  hd44780_wait_ready(false);

  /*
   * Enable display, activate blinking cursor.
   */
  hd44780_outcmd(HD44780_DISPCTL(1, 0, 0));
  hd44780_wait_ready(false);
}

/*
 * Send character c to the LCD display.  After a '\n' has been seen,
 * the next character will first clear the display.
 */
inline int lcd_putchar(char c)
{
  static bool nl_seen;

  if (nl_seen && c != '\n')
    {
      /*
       * First character after newline, clear display and home cursor.
       */
      hd44780_wait_ready(false);
      hd44780_outcmd(HD44780_CLR);
      hd44780_wait_ready(false);
      hd44780_outcmd(HD44780_HOME);
      hd44780_wait_ready(true);
      hd44780_outcmd(HD44780_DDADDR(0));

      nl_seen = false;
    }
  if (c == '\n')
    {
      nl_seen = true;
    }
  else
    {
      hd44780_wait_ready(false);
      hd44780_outdata(c);
    }
  _delay_ms(10);

  return 0;
}


void lcd_pos(uint8_t line, uint8_t pos)
{
	uint8_t location=0x80;
	if (line == 1 ){
		location |= (0x00 | pos);
	}
	else if (line == 2 ){
		location |= (0x40 | pos);
	}

	hd44780_outcmd(location);
    _delay_ms(20);

}

void lcd_putstring(char* String)
{
	while (*String != 0x00) {
	lcd_putchar(*String);
	String++;
	}
}

void lcd_putint (int i)
{
	char buf[8] = "";
	itoa (i,buf,10);
	lcd_putstring(buf);
}


void clear_screen(void){
	hd44780_outcmd(HD44780_CLR);
	hd44780_outcmd(HD44780_HOME);
    _delay_ms(10);

}
