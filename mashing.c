
#include <avr/io.h>	// deal with port registers
#include <util/delay.h>	// used for _delay_us function
#include <stdbool.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "mashing.h"
#include "defines.h"
#include "temp.h"
#include "heat.h"


#define HIGH 3
#define MED 2
#define LOW 1
#define OFF 0

bool targetReached=false;
bool nextStep;
volatile bool pause=false;
volatile uint16_t sec;
volatile uint16_t global_sec;
volatile bool autoMode=false;

void init_buttons(){
	init_switch();
	init_led();
	init_heater();
}



void start_mashing(uint8_t *target_temp, uint16_t *time_sec){

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
			sec=global_sec;
			wait(time_sec, target_temp);
			nextStep = true;
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

	cli();
		switch (level) {
		case HIGH:
			*working=5;
			*stopping=0;
			lcd_pos(4,9);
			lcd_putstring("H");
			break;
		case MED:
			*working=3;
			*stopping=2;
			lcd_pos(4,9);
			lcd_putstring("M");
			break;
		case LOW:
			*working=2;
			*stopping=3;
			lcd_pos(4,9);
			lcd_putstring("L");
			break;
		case OFF:
			*working=0;
			*stopping=5;
			lcd_pos(4,9);
			lcd_putstring("O");
			break;
		}
	sei();
}


void wait(uint16_t *seconds, uint8_t *temp ) {

	while ((pause) && (autoMode)){
		keepTemp(temp);
	}

//	lcd_pos(1,10);
//	lcd_putchar(' ');
//	lcd_putchar(' ');
//	lcd_putchar(' ');
//	lcd_putchar(' ');
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

void manual_heating(){
	while (!autoMode){
		if (SWITCH_ON){
		   start_heating();
		   SWITCH_LED_HIGH;
		}
		else{
		   stop_heating();
		   SWITCH_LED_LOW();
		}
		_delay_ms(100);
	}
}

void display_temp(void){

	int digit=0, decimal=0;
				therm_read_temperatureRAW(&digit,&decimal);
				lcd_putint(digit);
				lcd_putchar('.');
				lcd_putint(decimal/10);
}



