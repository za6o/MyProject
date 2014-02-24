/*
 * temp.c
 *
 *  Created on: Feb 20, 2014
 *      Author: za6opz
 */


#include <avr/io.h>	// deal with port registers
#include <util/delay.h>	// used for _delay_us function
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"


#define THERM_PORT			PORTD
#define THERM_DDR 			DDRD
#define THERM_PIN 			PIND
#define THERM_DQ 			PD0
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() 		THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() 		THERM_PORT|=(1<<THERM_DQ)



#define THERM_CMD_CONVERTTEMP    0x44
#define THERM_CMD_RSCRATCHPAD    0xbe
#define THERM_CMD_WSCRATCHPAD    0x4e
#define THERM_CMD_CPYSCRATCHPAD	  0x48
#define THERM_CMD_RECEEPROM	      0xb8
#define THERM_CMD_RPWRSUPPLY     0xb4
#define THERM_CMD_SEARCHROM	      0xf0
#define THERM_CMD_READROM	      0x33
#define THERM_CMD_MATCHROM	      0x55
#define THERM_CMD_SKIPROM	      0xcc
#define THERM_CMD_ALARMSEARCH	  0xec

#define THERM_DECIMAL_STEPS_12BIT	625



uint8_t therm_reset()
{
	uint8_t i;

	//Pull line low and wait for 480uS
	THERM_OUTPUT_MODE();
	THERM_LOW();
	_delay_us(480);
	THERM_INPUT_MODE();
	_delay_us(60);
	i=(THERM_PIN & (1<<THERM_DQ));
	_delay_us(420);
	return i;
}

void therm_write_bit(uint8_t bit)
{
	//Pull line low for 1uS
	THERM_OUTPUT_MODE();
	THERM_LOW();
	_delay_us(1);

	//If we want to write 1, release the line (if not will keep low)
	if(bit) THERM_INPUT_MODE();

	_delay_us(60);
	THERM_INPUT_MODE();
}

uint8_t therm_read_bit(void)
{
	uint8_t bit=0;
	//Pull line low for 1uS
	THERM_OUTPUT_MODE();
	THERM_LOW();
	_delay_us(1);

	//Release line and wait for 14uS
	THERM_INPUT_MODE();
	_delay_us(14);

	//Wait for 45uS to end and return read value
	if(THERM_PIN&(1<<THERM_DQ)) bit=1;
	_delay_us(45);

	return bit;
}


uint8_t therm_read_byte(void)
{
	uint8_t i=8, n=0;
	while(i--){
		n>>=1;
		n|=(therm_read_bit()<<7);
	}
	return n;
}

void therm_write_byte(uint8_t byte)
{
	uint8_t i=8;
	while(i--){
		therm_write_bit(byte&1);
		byte>>=1;
	}
}

void therm_read_temperature(uint8_t *digit, uint8_t *decimal){//char *buffer){

	uint8_t temperature[2];
	//int8_t digit;
	//uint16_t decimal;

	if (therm_reset())
		//sprintf(buffer, "1st reset isn't completed");

	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_CONVERTTEMP);
	while(!therm_read_bit());
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_RSCRATCHPAD);
	temperature[0]=therm_read_byte();
	temperature[1]=therm_read_byte();
	therm_reset();
	*digit=temperature[0]>>4;
	*digit|=(temperature[1]&0x7)<<4;
	*decimal=temperature[0]&0xf;
	*decimal*=THERM_DECIMAL_STEPS_12BIT;
	//return decimal;
}


