/*
 * timer.c
 *
 *  Created on: Feb 27, 2014
 *      Author: anaidenov
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void timer_config(void){
	 TCCR1B = (1<<WGM12)|(1<<CS12); // enable CTS mode; 1HZ=61499 clocks=256 prescale
	 OCR1A =  61499;            //sets the desired count
	 TIMSK1 = (1<<OCIE1A); 		//Enable timer interrupts
	 sei();    //Enable global interrupts, so our interrupt service routine can be called
}
