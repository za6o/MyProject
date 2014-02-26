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

	uint8_t digit;
	uint8_t decimal;
	char test[7];
	char test1[7];
	char test2[7];
	ioinit();
	//char String[] = "Test";

 for(;;){

	 	 	lcd_putstring("T:");
	 	    therm_read_temperature(&digit,&decimal);
			utoa (digit, test, 10);
			utoa (decimal, test1, 10);
			lcd_putstring(test);
			lcd_putchar('.');
			lcd_putstring(test);
			delay_1s();

			lcd_pos(1,8);
			lcd_putstring("t18");
			delay_1s();


			lcd_pos(2,0);
			lcd_putstring("t21");
			//----------------------------------------
			 	 	uint8_t tmp = 0b00011001;
			 	    utoa (tmp, test2, 10);
			 	   lcd_putstring(test2);//should print 25
			//----------------------------------------
			delay_1s();


			lcd_pos(2,8);
			lcd_putstring("t28");

/*			lcd_pos(1,8);
			therm_read_temperature(&digit,&decimal);
			lcd_putstring("T: ");
			itoa (digit, test, 10);
			lcd_putstring(test);
			lcd_putchar('.');
			itoa (decimal, test, 10);
			lcd_putstring(test);
			delay_1s();

			lcd_pos(2,1);
			therm_read_temperature(&digit,&decimal);
			lcd_putstring("T: ");
			itoa (digit, test, 10);
			lcd_putstring(test);
			lcd_putchar('.');
			itoa (decimal, test, 10);
			lcd_putstring(test);
			delay_1s();

			lcd_pos(2,8);
			therm_read_temperature(&digit,&decimal);
			lcd_putstring("T: ");
			itoa (digit, test, 10);
			lcd_putstring(test);
			lcd_putchar('.');
			itoa (decimal, test, 10);
			lcd_putstring(test);
			delay_1s();
*/

//	uart_putchar(" ");
 }



  return 0;
}
