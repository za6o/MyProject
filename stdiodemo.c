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

	uint8_t* next_temp=target_temp;
	uint16_t* next_sec=time_sec;

	clear_screen();
	lcd_pos(1,3);
	lcd_putstring("Temp:");

	lcd_pos(2,0);
	lcd_putstring("targ:");
	lcd_putint(*target_temp);
	lcd_putchar('C');

	lcd_pos(2,12);
	lcd_putstring("sec:");
	lcd_putint(*time_sec);


	lcd_pos(4,0);
	lcd_putstring("next:");
	lcd_putint(*(++next_temp));
	lcd_putchar('C');

	lcd_pos(4,11);
	lcd_putstring(" sec:");
	lcd_putint(*(++next_sec));

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
	}
	_delay_ms(800);
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

//------------ testing screen

    int l,p;
    for (l=1;l<=4; l++){
    	for (p=0; p<20; p++)
    	{
    		lcd_pos(l,p);
    		lcd_putint(p);
    		_delay_ms(400);
    	}
    }


//------------------------------------



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
		lcd_pos(1,0);
		lcd_putstring("                   "); // clear first line of the screen
		lcd_pos(1,0);
		lcd_putstring("Seconds for step ");
		lcd_putint(i+1);
		lcd_putstring("?");
		time_sec[i] = (uint16_t)menu();
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
			_delay_ms(2000);
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
	}
	//-----------------

	*target_temp=target_temp[0];
	*time_sec=time_sec[0];

	clear_screen();
	lcd_pos(2,4);
	lcd_putstring("Starting...");
    _delay_ms(1000);

    int n;
	for(n=0;n<=(steps-1);){

		if (!autoMode)
			manual_mode();
		else
			auto_mode();

		if (targetReached){
			target_temp++;
			time_sec++;
			n++;
			global_sec=0;
			sec=0;
			targetReached=false;
		}
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
	}
	else if (!autoMode){
		BUTT_LED_LOW();
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
		lcd_pos(1,16);
		lcd_putint(sec);
		if ((sec >= *time_sec)&&(!SWITCH_ON)){
			targetReached = true;
			pause=false;
		}
	}
}



