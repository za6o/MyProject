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

uint8_t target_temp[5] = {22,23,24,25,27};
uint16_t time_sec[5] = {5,10,11,12,13};
uint8_t* targ_temp = target_temp;
uint16_t* tim_sec = time_sec;

/*
 * Do all the startup-time peripheral initializations.
 */
static void ioinit(void)
{
  lcd_init();
  init_heater();
  int_config(); // enable timer interrupts
}


void precondition(void){
	lcd_pos(2,4);
	lcd_putint(*targ_temp);

	lcd_pos(2,12);
	lcd_putint(*tim_sec);

	nextStep=false;
	start_mashing(targ_temp, tim_sec);

	targ_temp++;
	tim_sec++;
}

int main(void) {


	PORTD |= (1<<PD2);

	ioinit();

	uint8_t cycles=sizeof(target_temp)/sizeof(uint8_t);

	lcd_putstring("Starting...");
    _delay_ms(900);

	uint8_t i;
	for(i=0;i<cycles; i++){
		clear_screen();
		lcd_putstring("T:");
		//display_temp();
		lcd_pos(2,0);
		lcd_putstring("tar:");

		lcd_pos(2,8);
		lcd_putstring("sec:");

		precondition();
	}
	clear_screen();
	lcd_pos(1,3);
	lcd_putstring("FINISHED");

	return 0;
}

ISR(INT0_vect) {

	_delay_ms(20);
	if (PIND&(1<<PD2)){
		button_pressed=!button_pressed;
	}

	if (!button_pressed){
		button_pressed = true;
		BUTT_LED_HIGH();
		lcd_pos(1,13);
		lcd_putchar('T');
	}
	else if (button_pressed){
		button_pressed = false;
		BUTT_LED_LOW();
		lcd_pos(1,13);
				lcd_putchar('F');
	}
}


ISR(TIMER1_COMPA_vect){

	sec++;
	if (sec >65000)
		sec=0;

	lcd_pos(1,2);
	display_temp();

	if (pause){
		lcd_pos(1,10);
		lcd_putint(sec);
		if (sec >= *tim_sec){
			pause=false;
		}
	}
}
