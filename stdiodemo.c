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
#include <avr/interrupt.h>

#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#include <util/delay.h>

#include "lcd.h"
#include "timer.h"
#include "mashing.h"
#include "keyboard.h"


static uint8_t *target_temp=NULL;
static uint16_t *time_sec=NULL;

/*
 * Do all the startup-time peripheral initializations.
 */
static inline void ioinit(void)
{
	init_keyboard();
	lcd_init();
	init_buttons();
    init_timers(); // enable timer interrupts

}

static void auto_mode(){
	cli();
	clear_screen();
	lcd_pos(1,3);
	lcd_putstring("Temp:");

	lcd_pos(2,0);
	lcd_putstring("targ:");
	lcd_pos(2,7);
	lcd_putchar('C');

	lcd_pos(2,12);
	lcd_putstring("sec:");

	lcd_pos(4,0);
	lcd_putstring("next:");
	lcd_pos(4,7);
	lcd_putchar('C');

	lcd_pos(4,12);
	lcd_putstring("sec:");

	lcd_pos(2,5);
	lcd_putint(*target_temp);

	lcd_pos(2,16);
	lcd_putint(*time_sec);

	lcd_pos(4,5);
	lcd_putint(*(++target_temp));

	lcd_pos(4,16);
	lcd_putint(*(++time_sec));

	nextStep=false;

	sei();
	start_mashing(target_temp, time_sec);
}

static void manual_mode(){
	cli();

	global_sec=sec;
	clear_screen();
	lcd_pos(1,3);
	lcd_putstring("Temp:");
	lcd_pos(2,4);
	lcd_putstring("Manual Mode");

	sei();

	manual_heating();
}


static uint32_t menu(void){

	uint32_t RealValue=0;
	uint8_t PresKey=0xFF;

	lcd_pos(2,0);
	lcd_putstring("Input:");
	while (GetKey(&PresKey)){
		if(PresKey==0xF0){
			RealValue=RealValue/10; // if * button is pressed, one position back
		}
		else {
			RealValue=(uint32_t)((RealValue*10)+(PresKey)); //(4<<RealValue(<<4)) | (PresKey);
		}
		lcd_pos(2,6);
		lcd_putstring("             "); // cleaning rest of the line
		lcd_pos(2,6);
		lcd_putint(RealValue);
		_delay_ms(150);
	}
	_delay_ms(600);
	lcd_pos(2,6);
	lcd_putstring("             "); // cleaning rest of the line
	return RealValue;
}

int main(void) {

	ioinit();
    global_sec=0;
    sec=0;
	uint16_t steps;

	lcd_pos(2,6);
	lcd_putstring("WELLCOME");
    _delay_ms(1500);

    clear_screen();
    lcd_pos(1,1);
	lcd_putstring("Steps for mashing?");

	while((steps = (uint16_t)menu())==0){
		lcd_putstring("Insert one step at least!");
	    clear_screen();
	    lcd_pos(1,1);
		lcd_putstring("Steps for mashing?");

	}

	target_temp =(uint8_t*)malloc(sizeof(*target_temp)*steps);
	time_sec = (uint16_t*)malloc(sizeof(*time_sec)*steps);

	if ((target_temp==NULL) || time_sec==NULL )
	{
		lcd_putstring("Couldn't allocate buffers... restart it ");
	}

	uint8_t i;
	for (i=0;i<steps;i++)
	{
		clear_screen();
		if (i > 0){
			lcd_pos(4,0);
			lcd_putstring("temp:");
			lcd_putint(target_temp[i-1]);

			lcd_pos(4,12);
			lcd_putstring("sec:");
			lcd_putint(time_sec[i-1]);
		}
		lcd_pos(1,0);
		lcd_putstring("Celsius for step ");
		lcd_putint(i+1);
		lcd_putstring("?");
		target_temp[i] = (uint8_t)menu();
		clear_screen();
		lcd_putstring("Seconds for step ");
		lcd_putint(i+1);
		lcd_putstring("?");
		time_sec[i] = (uint16_t)menu();
		_delay_ms(1000);

	}

	*target_temp=target_temp[0];
	*time_sec=time_sec[0];

	//*** for debugging**

	clear_screen();
	for (i=0;i<steps;i++)
	{
		uint8_t line;

		if (i<4)
			line=i+1;
		else if((i%4)==0){
			line = 1;
			clear_screen();
		}
		else
			line = (i%4)+1;

		lcd_pos(line,0);
		lcd_putint(i+1);
		lcd_putstring("- ");
		lcd_putstring("temp:");
		lcd_putint(target_temp[i]);
		lcd_putstring("  ");
		lcd_putstring("sec:");
		lcd_putint(time_sec[i]);

		_delay_ms(1500);
		_delay_ms(1500);
	}
	//-----------------

	*target_temp=target_temp[0];
	*time_sec=time_sec[0];

	clear_screen();
	lcd_pos(2,4);
	lcd_putstring("Starting...");
    _delay_ms(1000);

	sei();    //Enable global interrupts, so our interrupt service routine can be called

	for(i=0;i<(steps-1);){

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

	lcd_putstring("FINISHED");

	free(target_temp);
	free(target_temp);
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

	lcd_pos(1,8);
	display_temp();

	if(SWITCH_ON){
		SWITCH_LED_HIGH;
	}
	else
		SWITCH_LED_LOW();

	if (pause){
		sec++;
		if (((*target_temp)-sec) < 10){
			lcd_pos(2,17);
			lcd_putstring("   ");
		}
		else if (((*target_temp)-sec) < 100){
			lcd_pos(2,18);
			lcd_putstring("  ");
		}
		else if (((*target_temp)-sec) < 1000){
			lcd_pos(2,19);
			lcd_putstring(" ");
		}

		lcd_pos(2,16);
		lcd_putint((*target_temp)-sec);
		if ((sec >= *time_sec)&&(!SWITCH_ON)){
			targetReached = true;
			pause=false;
		}
	}
}



