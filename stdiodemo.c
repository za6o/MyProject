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
//#include "timer.h"


//----------------------

uint8_t target_temp1 = 40;
uint16_t time_sec1 = 10;

//-----------------------


/*
 * Do all the startup-time peripheral initializations.
 */
static void ioinit(void)
{
  uart_init();
  lcd_init();
}

static void delay_1s(void)
{
  uint8_t i;

  for (i = 0; i < 100; i++)
    _delay_ms(10);
}

int main(void) {

	ioinit();

	lcd_putstring("Starting...");
	delay_1s();
	delay_1s();

	clear_screen();
	lcd_putstring("Temp:");
	display_temp();

	lcd_pos(2,0);
	lcd_putstring("targ:");
	lcd_putint(target_temp1);

	lcd_pos(2,8);
	lcd_putstring("time:");
	lcd_putint(time_sec1);

//	timer_config(); // enable timer interrupts

	init_heater();

	for(;;){
		lcd_pos(1,5);
		if(compare(target_temp1)){
			stop_heating();
		}
		else
			start_heating();

		delay_1s();
	}

//	start_mashing(target_temp1, time_sec1);

	return 0;
}

//ISR(TIMER1_COMPA_vect){

//}
