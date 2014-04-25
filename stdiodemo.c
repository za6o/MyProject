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

uint8_t *target_temp;
uint16_t *time_sec;

/*
 * Do all the startup-time peripheral initializations.
 */
static inline void ioinit(void)
{
  lcd_init();
  init_LedsSwitch();
#if 0
  init_heater();
  init_led();
  init_switch();
#endif
  int_config(); // enable timer interrupts
  PORTD |= (1<<PD2); // enable pullUP for the push button

}

void static precondition(void){
	lcd_pos(2,4);
	lcd_putint(*target_temp);

	lcd_pos(2,12);
	lcd_putint(*time_sec);

	nextStep=false;
}

void auto_mode(){
	cli();
	clear_screen();
	lcd_putstring("T:");

	lcd_pos(2,0);
	lcd_putstring("tar:");

	lcd_pos(2,8);
	lcd_putstring("sec:");

	lcd_pos(2,4);
	lcd_putint(*target_temp);

	lcd_pos(2,12);
	lcd_putint(*time_sec);
	precondition();
	sei();
	start_mashing(target_temp, time_sec);
}

void manual_mode(){
	cli();

	global_sec=sec;
	clear_screen();
	lcd_putstring("T:");
	lcd_pos(2,2);
	lcd_putstring("Manual Mode");

	sei();
	while (!autoMode){
		if (SWITCH_ON){
		   start_heating();
		   SWITCH_LED_HIGH();
		}
		else{
		   stop_heating();
		   SWITCH_LED_LOW();
		}
		_delay_ms(100);
	}
}


int main(void) {

	ioinit();


	uint8_t steps=5;
	target_temp = malloc(sizeof(*target_temp)*steps);
	time_sec = malloc(sizeof(*time_sec)*steps);

	uint8_t cycles=sizeof(target_temp)/sizeof(uint8_t);

	*target_temp=20;
	*time_sec=10;

	for (;cycles<0;cycles--)
	{
		*target_temp +=2;
		*time_sec +=5;
		target_temp++;
		time_sec++;
	}


	lcd_putstring("Starting...");
    _delay_ms(900);

    global_sec=0;
    sec=0;

	sei();    //Enable global interrupts, so our interrupt service routine can be called

	uint8_t i;
	for(i=0;i<cycles;){

		if (targetReached){
			target_temp++;
			time_sec++;
			i++;
			global_sec=0;
			sec=0;
		}

		if (!autoMode)
			manual_mode();

		auto_mode();
	}

	cli();
	clear_screen();
	lcd_pos(1,3);
	lcd_putstring("FINISHED");
	_delay_ms(900);

	return 0;
}

ISR(INT0_vect) {

	_delay_ms(20);

	if (bit_is_clear(PIND,PD2)){
	autoMode=!autoMode;
	pause=false;
	}

	if (autoMode){
		BUTT_LED_HIGH();
		//SWITCH_LED_LOW();
		targetReached=false;
	}
	else if (!autoMode){
		BUTT_LED_LOW();
		targetReached=false;
	}

}

ISR(TIMER1_COMPA_vect){

	lcd_pos(1,2);
	display_temp();

	if(SWITCH_ON){
		SWITCH_LED_HIGH();
	}
	else
		SWITCH_LED_LOW();

	if (pause){
		sec++;
		lcd_pos(1,10);
		lcd_putint(sec);
		if ((sec >= *time_sec)&&(!SWITCH_ON)){
			targetReached = true;
			pause=false;
		}
	}
}



