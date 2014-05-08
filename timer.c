/*
 * timer.c
 *
 *  Created on: Feb 27, 2014
 *      Author: anaidenov
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/*
 * Target Timer Count = (((Input Frequency / Prescaler) / Target Frequency) - 1)
 */

void int_config(void){

	/*
	 *  button interrupts INT0
	 */
	EICRA = (1<<ISC01); // falling edge INT0
	EIMSK = (1<<INT0); // enable INT0


	 TCCR1B = (1<<WGM12)|(1<<CS12); // enable CTS mode; 1HZ=62499 clocks=256 prescale
	 OCR1A = 62499;    //sets the desired count for displaying temp every one sec
	 TIMSK1 = (1<<OCIE1A); //Enable timer interrupt for "A" timer
}

