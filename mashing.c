
#include <avr/io.h>	// deal with port registers
#include <util/delay.h>	// used for _delay_us function
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd.h"
#include "temp.h"
#include "heat.h"

#ifndef stepmashing

inline void remaining_sec(uint16_t time, uint16_t desired_time)
{
	uint16_t diff=desired_time-time;
	if (diff <= 99){
		lcd_pos(2,15);
		lcd_putchar(' ');
	}
	if (diff <= 9){
		lcd_pos(2,14);
		lcd_putchar(' ');
	}

    lcd_pos(1,13);
    lcd_putint(diff);

}
#endif
#ifdef stepmashing

#define HIGH 3
#define MED 2
#define LOW 1
#define OFF 0

static bool pause=false;

uint8_t speedSelect(uint8_t *target){

	int actualDigit=0
	therm_read_temperatureRAW(&actualDigit,&decimal);
//	lcd_putint(digit);
//	lcd_putchar('.');
//	lcd_putint(decimal/100);

	if (((*target)-actualDigit)>5)
		return level = HIGH;
	else if (1>((*target)-actualDigit)>5)
		return level = MED;
	else if (0>=((*target)-actual)>1)
		return level = LOW;
	else{
		return level = OFF;
		pause = true
	}
}

#endif
#ifndef stepmashing

inline uint8_t compare(uint8_t targT){

	int digit=0, decimal=0;

	therm_read_temperatureRAW(&digit,&decimal);
	lcd_putint(digit);
	lcd_putchar('.');
	lcd_putint(decimal/100);

	if ((int)targT <= digit)
		return 1;
	else
		return 0;


}

inline uint8_t step_mashing(uint8_t target_temp){

		lcd_pos(1,5);
		if(compare(target_temp)){
			stop_heating();
			return 1;
		}
		else
			start_heating();

		return 0;
}

uint8_t reaching_targ(uint8_t targ_temp)
{
	return (step_mashing(targ_temp));
}

uint8_t wait_time (uint8_t target_temp, uint16_t desired_time,uint16_t sec)
{
	step_mashing(target_temp);

	if(sec > desired_time){
		return 1;
	}
	else{
		remaining_sec(sec, desired_time);
		return 0;
	}

}

#endif

#ifdef stepmashing

uint8_t wait(uint16_t *seconds ) {
	extern uint16_t sec=0;
	while (sec < *seconds ){
      // keep the temp in the range //speedSelect
	}
	return 1;
}

void startHeating (uint8_t working,uint8_t stopping){
	local_sec = sec;

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

setSpeed(uint8_t level){
	uint8_t ON=0, OFF=0;
	switch (level) {
	case HIGH:
		ON=5;
		OFF=0;
		break;
	case MED:
		ON=3;
		OFF=2;
		break;
	case LOW:
		ON=1;
		OFF=4;
		break;
	case OFF:
		ON=0;
		OFF=5;
		break;
	}

	startHeating (ON, OFF);

}

void start_mashing(uint8_t *target_temp, uint16_t *time_sec){

	uint8_t cycles;
	for (i=0; i < cycles; i++){
		*(target_temp+i);
		*(time_sec+i);

		switch (speedSelect(&target_temp)){
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
			//if (pause)
			wait(&time_sec);
			break;
		default: // just for debugging
			clear_screen();
			lcd_putstring("default switch statement!!!");
			break;
		}
	}

	*(target_temp++);

#endif

