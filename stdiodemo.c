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
#include <avr/interrupt.h>

#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include "lcd.h"
#include "uart.h"
#include "temp.h"
#include "mashing.h"
#include "timer.h"

/*
 * Do all the startup-time peripheral initializations.
 */
static void ioinit(void)
{
  uart_init();
  lcd_init();
  timer_config();
}

static void delay_1s(uint8_t sec)
{
  uint8_t i;

  for (i = 0; i < (100*sec); i++)
    _delay_ms(10);
}

int main(void) {

	uint8_t target_temp=40;
	uint16_t time_sec=10;

	int digit=0, decimal=0;
	ioinit();

			lcd_putstring("Start Mashing");
			delay_1s(5);

			clear_screen();
	 	 	lcd_putstring("Temp:");
	 	    therm_read_temperatureRAW(&digit,&decimal);
			lcd_putint(digit);
			lcd_putchar('.');
			lcd_putint(decimal/100);

			lcd_pos(2,0);
			lcd_putstring("targ:");
			lcd_putint(target_temp);

			lcd_pos(2,8);
			lcd_putstring("time:");
			lcd_putint(time_sec);

			start_mashing(target_temp, time_sec);

  return 0;
}

ISR(TIMER1_COMPA_vect){

	lcd_pos(1,5);
	therm_read_temperature();

}
