/*
 * mashing.c
 *
 *  Created on: Feb 27, 2014
 *      Author: anaidenov
 */


#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <avr/io.h>

#include <util/delay.h>

static bool heater_on = false;

volatile bool button_pressed=false;
volatile bool manul_mode = false;


void init_heater(){
	HEAT_OUTPUT_MODE();
	BUTT_LED_OUTPUT();
	HEAT_LOW();
	BUTT_LED_LOW();
}

void stop_heating(){
	if(heater_on){
		HEAT_LOW();
		heater_on=false;
	}
}

void start_heating(){
	if ((button_pressed) || (!button_pressed && manul_mode)){
		if(!heater_on){
			HEAT_HIGH();
			heater_on=true;
		}
	}
}
