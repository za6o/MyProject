/*
 * mashing.h
 *
 *  Created on: Feb 28, 2014
 *      Author: anaidenov
 */

#ifndef __MASHING_H__
#define __MASHING_H__


extern volatile uint16_t sec;

uint8_t speedSelect(uint8_t*);

uint8_t wait(uint16_t*);

void startHeating (uint8_t ,uint8_t );

void setSpeed(uint8_t);

void start_mashing( const uint8_t *,const uint16_t *);

#endif


