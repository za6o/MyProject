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



//volatile uint16_t sec;

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

	 uint8_t target_temp[5] = {25,35,65,72,78};
	 uint16_t time_sec[5] = {5,30,120,600,10};
	 uint8_t* targ_temp = target_temp;
	 uint16_t* tim_sec = time_sec;

	uint8_t cycles=sizeof(target_temp)/sizeof(uint8_t);

	ioinit();

	lcd_putstring("Starting...");
    _delay_ms(900);

	clear_screen();
	lcd_putstring("Temp:");
	display_temp();



	uint8_t i;
	for(i=0;i<=cycles; i++){
		lcd_pos(2,0);
		lcd_putstring("targ:");
		lcd_putint(*targ_temp);

		lcd_pos(2,8);
		lcd_putstring("time:");
		lcd_putint(*tim_sec);

		start_mashing(targ_temp, tim_sec);

		targ_temp++;
		tim_sec++;
	}
	return 0;
}

ISR(TIMER1_COMPA_vect){

	sec++;
	if (sec >65000)
		sec=0;

	lcd_pos(1,5);
	display_temp();

	if (pause){
		lcd_pos(1,14);
		lcd_putint(sec);
	}

}
