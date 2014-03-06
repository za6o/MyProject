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
	if(!heater_on){
		HEAT_HIGH();
		heater_on=true;
	}
}
