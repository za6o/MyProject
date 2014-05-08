/*
 * mashing.c
 *
 *  Created on: Feb 27, 2014
 *      Author: anaidenov
 */


#include <stdbool.h>

#include <avr/io.h>

#include "defines.h"
#include "mashing.h"


static bool heater_on = false;

void init_switch(){
	SWITCH_INPUT();
	SWITCH_LED_OUTPUT();
	SWITCH_LED_LOW();
}

void init_led(){
	BUTT_LED_OUTPUT();
	BUTT_LED_LOW();
}

void init_heater(){
	HEAT_OUTPUT_MODE();
	HEAT_LOW();
}

void stop_heating(){
	if(heater_on){
		HEAT_LOW();
		heater_on=false;
	}
}

void start_heating(){

	if ((autoMode) || ((!autoMode)&&(SWITCH_ON))){
		if(!heater_on){
			HEAT_HIGH();
			heater_on=true;
		}
	}
}

