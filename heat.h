/*
 * mashing.h
 *
 *  Created on: Feb 27, 2014
 *      Author: anaidenov
 */


#ifndef __HEAT_H__
#define __HEAT_H__

extern volatile bool button_pressed;
extern volatile bool manualMode;

void init_heater(void);

void init_led(void);

void init_switch(void);

void stop_heating(void);

void start_heating(void);

#endif
