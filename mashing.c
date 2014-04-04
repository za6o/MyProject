
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

bool targetReached=false;
bool nextStep;
volatile bool pause=false;
volatile uint16_t sec;



void start_mashing(uint8_t *target_temp, uint16_t *time_sec){

//	uint8_t reachTemp = (*target_temp)+1;
	while ((!nextStep) && (autoMode)){
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
			pause = true;
			wait(time_sec, target_temp);
			break;
		}
	}
}

inline uint8_t speedSelect(uint8_t *target){

	int actualDigit=0;
	int decimal=0;
	therm_read_temperatureRAW(&actualDigit,&decimal);

	if ((*target-actualDigit)>2)
		return HIGH;
	else if (((*target-actualDigit))>1 && ((*target-actualDigit)<=2))
		return MED;
	else if  ((*target-actualDigit)==1)
		return LOW;
	else
		return OFF;
}

void startHeating (uint8_t level){

	uint8_t i;
	uint8_t working=0, stopping=0;

	setSpeed (level, &working, &stopping);

	if (autoMode){
		for (i=0; i < working ; i ++){
			start_heating();
			_delay_ms(800);
		}
	}

	if(autoMode){
		for (i=0; i < stopping ; i ++){
			stop_heating();
			_delay_ms(800);
		}
	}
}

void setSpeed (uint8_t level,uint8_t *working,uint8_t *stopping){

		switch (level) {
		case HIGH:
			*working=5;
			*stopping=0;
			lcd_pos(1,15);
			lcd_putstring("H");
			break;
		case MED:
			*working=3;
			*stopping=2;
			lcd_pos(1,15);
					lcd_putstring("M");
			break;
		case LOW:
			*working=2;
			*stopping=3;
			lcd_pos(1,15);
					lcd_putstring("L");
			break;
		case OFF:
			*working=0;
			*stopping=5;
			lcd_pos(1,15);
					lcd_putstring("O");
			break;
		}
}


void wait(uint16_t *seconds, uint8_t *temp ) {

	sec=0;

	while ((pause) && (autoMode)){
		keepTemp(temp);
	}

	nextStep = true;
	lcd_pos(1,10);
	lcd_putchar(' ');
	lcd_putchar(' ');
	lcd_putchar(' ');
	lcd_putchar(' ');
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




