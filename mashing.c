
#include <avr/io.h>	// deal with port registers
#include <util/delay.h>	// used for _delay_us function
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd.h"
#include "temp.h"
#include "heat.h"
#include "mashing.h"


#define HIGH 3
#define MED 2
#define LOW 1
#define OFF 0

bool nextStep;
volatile bool pause=false;
volatile uint16_t sec;


void start_mashing(uint8_t *target_temp, uint16_t *time_sec){

	while (!nextStep){
		switch (speedSelect(target_temp)){
		case HIGH:
			startHeating(HIGH);
			break;
		case MED:
			startHeating(MED);
			break;
		case LOW:
			startHeating(LOW);
			break;
		case OFF:
			startHeating(OFF);
			pause = true;
			wait(time_sec, target_temp);
			break;
		default: // just for debugging
			clear_screen();
			lcd_putstring("default switch statement!!!");
			break;
		}
	}
}

uint8_t speedSelect(uint8_t *target){

	int actualDigit=0;
	int decimal=0;
	therm_read_temperatureRAW(&actualDigit,&decimal);
//	lcd_putint(digit);
//	lcd_putchar('.');
//	lcd_putint(decimal/100);

	if (((*target)-actualDigit)>3)
		return HIGH;
	else if (((*target-actualDigit))>1 && ((*target-actualDigit)<=3))
		return MED;
	else if  ((*target-actualDigit)==1)
		return LOW;
	else
		return OFF;
}

inline void startHeating (uint8_t level){

	uint8_t i;
	uint8_t working=0, stopping=0;

	setSpeed (level, &working, &stopping);

	for (i=0; i < working ; i ++){
		start_heating();
		_delay_ms(500);
	}
	for (i=0; i < stopping ; i ++){
		stop_heating();
		_delay_ms(500);
	}

}

void setSpeed (uint8_t level,uint8_t *working,uint8_t *stopping){

		switch (level) {
		case HIGH:
			*working=5;
			*stopping=0;
			lcd_pos(1,11);
			lcd_putstring("h");
			break;
		case MED:
			*working=3;
			*stopping=2;
			lcd_pos(1,11);
					lcd_putstring("m");
			break;
		case LOW:
			*working=2;
			*stopping=3;
			lcd_pos(1,11);
					lcd_putstring("l");
			break;
		case OFF:
			*working=0;
			*stopping=5;
			lcd_pos(1,11);
					lcd_putstring("o");
			break;
		}
}


void wait(uint16_t *seconds, uint8_t *temp ) {

	sec=0;

	while (pause){
		keepTemp(temp);//   keep the temp in the range //speedSelect
	}
	nextStep = true;
	lcd_pos(1,14);
	lcd_putint(0);
	lcd_putint(0);
}

void keepTemp(uint8_t *tempereture){


	switch (speedSelect(tempereture)){
				case HIGH:
					startHeating(HIGH);
					break;
				case MED:
					startHeating(MED);
					break;
				case LOW:
					startHeating(LOW);
					break;
				case OFF:
					startHeating(OFF);
					break;
	}
}




