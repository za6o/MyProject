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
#include "uart.h"
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
  uart_init();
  lcd_init();
  init_heater();
  timer_config(); // enable timer interrupts
}

static void delay_1s(void)
{
  uint8_t i;

  for (i = 0; i < 100; i++)
    _delay_ms(10);
}

int main(void) {

	const uint8_t target_temp[5] = {22,23};
	const uint16_t time_sec[5] = {5,20};
	bool heating=true;
	uint8_t cycle=0;

	ioinit();

	lcd_putstring("Starting...");
	delay_1s();
	//delay_1s();

	clear_screen();
	lcd_putstring("Temp:");
	display_temp();

	lcd_pos(2,0);
	lcd_putstring("targ:");
	lcd_putint(target_temp[cycle]);
//	delay_1s();

	lcd_pos(2,8);
	lcd_putstring("time:");
	lcd_putint(time_sec[cycle]);
//	delay_1s();

	custom_character(0x1f, 0x11, 0x17, 0x11, 0x1f, 0x0, 0, 0);
//	custom_character(0x1f, 0x11, 0x15, 0x15, 0x1f, 0x0, 0, 0);
//	custom_character(0x1f, 0x11, 0x1d, 0x11, 0x1f, 0x0, 0, 0);
//	custom_character(0x1f, 0x15, 0x15, 0x11, 0x1f, 0x0, 0, 0);
	lcd_pos(1,15);
	lcd_putchar(0);


	for(;;){
		if (heating){
			if (step_mashing(target_temp[cycle])){
				sec=0;
				heating=false;
			}
		}
		else
			if (wait_time(target_temp[cycle],time_sec[cycle], sec)){
				heating=true;
				cycle++;

				clear_screen();
				lcd_pos(2,0);
				//delay_1s();
				lcd_putstring("targ:");
				lcd_putint(target_temp[cycle]);

				lcd_pos(2,8);
				delay_1s();
				lcd_putstring("time:");
				lcd_putint(time_sec[cycle]);
				delay_1s();
			}
	}
	return 0;
}

ISR(TIMER1_COMPA_vect){

	sec++;
	if (sec >65000 )
		sec=0;
}
