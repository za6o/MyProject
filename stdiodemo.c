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

	uint8_t i;
	char test[5];
	ioinit();
	//char String[] = "Test";

 for(;;){
	for (i=0;i<6;i++)
		{
		therm_read_temperature(test);
		//	itoa(therm_read_temperature(),test,16);
			USART_putstring("Temp:");
			USART_putstring(test);
			delay_1s();
		}
	lcd_putstring(test);

//	uart_putchar(" ");
 }



  return 0;
}
