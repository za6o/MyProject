/*
 * mashing.h
 *
 *  Created on: Feb 28, 2014
 *      Author: anaidenov
 */

#ifndef __MASHING_H__
#define __MASHING_H__


extern volatile uint16_t sec;
extern volatile uint16_t global_sec;
extern volatile bool pause;
extern volatile bool autoMode;
extern bool nextStep;
extern bool targetReached;

uint8_t speedSelect(uint8_t*);

void wait(uint16_t*, uint8_t*);

void startHeating (uint8_t);

void setSpeed(uint8_t, uint8_t *, uint8_t *);

void start_mashing(  uint8_t *, uint16_t *);

void keepTemp(uint8_t* );

void manual_heating(void);

void display_temp(void); // print temp to display

void init_buttons(void);

#endif


