/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * Stdio demo
 *
 * $Id: stdiodemo.c 1008 2005-12-28 21:38:59Z joerg_wunsch $
 */

#include "defines.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include "lcd.h"
#include "uart.h"
#include "temp.h"

/*
 * Do all the startup-time peripheral initializations.
 */
static void
ioinit(void)
{
  uart_init();
  lcd_init();
}

static void
delay_1s(void)
{
  uint8_t i;

  for (i = 0; i < 100; i++)
    _delay_ms(10);
}

int main(void) {

	int just_test=0;
	int digit=0, decimal=0;
	ioinit();

 for(just_test=0;just_test<10; just_test++){

	 	 	lcd_putstring("T:");
	 	    therm_read_temperature(&digit,&decimal);

			lcd_putint(digit);
			lcd_putchar('.');
			lcd_putint(decimal);
			delay_1s();

			lcd_pos(2,0);
			lcd_putint(just_test);
			delay_1s();


 }



  return 0;
}
