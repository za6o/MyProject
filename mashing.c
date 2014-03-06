
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

bool pause=false;

volatile uint16_t sec;


void start_mashing(uint8_t *target_temp, uint16_t *time_sec){

	while (!pause){
		switch (speedSelect(target_temp)){
		case HIGH:
			setSpeed(HIGH);
			break;
		case MED:
			setSpeed(MED);
			break;
		case LOW:
			setSpeed(LOW);
			break;
		case OFF:
			setSpeed(OFF);
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

	if (((*target)-actualDigit)>5)
		return HIGH;
	else if (((*target-actualDigit))>1 && ((*target-actualDigit)<3))
		return MED;
	else if (((*target-actualDigit)>=0) && ((*target-actualDigit)<=1))
		return LOW;
	else
		return OFF;
}

void setSpeed(uint8_t level){

	uint8_t switchedON=0;
	uint8_t switchedOFF=0;

	switch (level) {
	case HIGH:
		switchedON=5;
		switchedOFF=0;
		break;
	case MED:
		switchedON=3;
		switchedOFF=2;
		break;
	case LOW:
		switchedON=2;
		switchedOFF=3;
		break;
	case OFF:
		switchedON=0;
		switchedOFF=5;
		break;
	}
	startHeating (switchedON, switchedOFF);
}

void startHeating (uint8_t working,uint8_t stopping){

	//uint16_t local_sec = sec;

	uint8_t i;
	for (i=0; i < working ; i ++){
		start_heating();
		_delay_ms(950);
	}
	for (i=0; i < stopping ; i ++){
			stop_heating();
			_delay_ms(950);
	}
}


void wait(uint16_t *seconds, uint8_t *temp ) {
	//extern uint16_t
	sec=0;
	while (sec < *seconds ){
		keepTemp(temp);//   keep the temp in the range //speedSelect
	}

}

void keepTemp(uint8_t *tempereture){


	switch (speedSelect(tempereture)){
				case HIGH:
					setSpeed(HIGH);
					break;
				case MED:
					setSpeed(MED);
					break;
				case LOW:
					setSpeed(LOW);
					break;
				case OFF:
					setSpeed(OFF);
					break;
	}
}




