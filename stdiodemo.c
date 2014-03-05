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
#include <stdbool.h>

#include <util/delay.h>

#include "lcd.h"
#include "temp.h"
#include "heat.h"
#include "timer.h"
#include "mashing.h"



volatile uint16_t sec=0;

/*
 * Do all the startup-time peripheral initializations.
 */
static void ioinit(void)
{
  lcd_init();
  init_heater();
  timer_config(); // enable timer interrupts
}

int main(void) {

	const uint8_t target_temp[5] = {40,55,65,72,78};
	const uint16_t time_sec[5] = {5,30,120,600,10};
	bool heating=true;
	uint8_t cycle=0;

	ioinit();

	lcd_putstring("Starting...");
    _delay_ms(900);

	clear_screen();
	lcd_putstring("Temp:");
	display_temp();

	lcd_pos(2,0);
	lcd_putstring("targ:");
	lcd_putint(target_temp[cycle]);

	lcd_pos(2,8);
	lcd_putstring("time:");
	lcd_putint(time_sec[cycle]);


	for(;;){
		if (heating){
			if (reaching_targ(target_temp[cycle])){
				sec=0;
				heating=false;
			}
		}
		else
			if (wait_time(target_temp[cycle],time_sec[cycle], sec)){
				heating=true;
				cycle++;

				lcd_pos(2,5);
				lcd_putint(target_temp[cycle]);
				lcd_pos(2,13);
				lcd_putint(time_sec[cycle]);
			}
	    _delay_ms(500);
	}
	return 0;
}

ISR(TIMER1_COMPA_vect){

	sec++;
	if (sec >65000 )

		sec=0;
}
