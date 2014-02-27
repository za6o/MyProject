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


#define THERM_PORT PORTD
#define THERM_DDR DDRD
#define THERM_PIN PIND
#define THERM_DQ PD2
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)



#define THERM_CMD_CONVERTTEMP    0x44
#define THERM_CMD_RSCRATCHPAD    0xbe
#define THERM_CMD_WSCRATCHPAD    0x4e
#define THERM_CMD_CPYSCRATCHPAD	 0x48
#define THERM_CMD_RECEEPROM	     0xb8
#define THERM_CMD_RPWRSUPPLY     0xb4
#define THERM_CMD_SEARCHROM	     0xf0
#define THERM_CMD_READROM	     0x33
#define THERM_CMD_MATCHROM	     0x55
#define THERM_CMD_SKIPROM	     0xcc
#define THERM_CMD_ALARMSEARCH	 0xec

#define THERM_DECIMAL_STEPS_12BIT	625



uint8_t therm_reset()
{
	uint8_t i;
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(480);
	THERM_INPUT_MODE();
	_delay_us(60);
	i=(THERM_PIN & (1<<THERM_DQ));
	_delay_us(420);
	return i;
}

void therm_write_bit(uint8_t bit)
{
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	if(bit) THERM_INPUT_MODE();
	_delay_us(60);
	THERM_INPUT_MODE();
}

uint8_t therm_read_bit(void)
{
	uint8_t bit=0;
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	THERM_INPUT_MODE();
	_delay_us(14);
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

void therm_read_temperatureRAW(int *digit, int *decimal){

//	uint8_t temperature[2];
	uint8_t firstbyte;
	uint8_t secondbyte;

    therm_reset();

	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_CONVERTTEMP);
	while(!therm_read_bit());
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_RSCRATCHPAD);
	firstbyte=therm_read_byte();
	secondbyte=therm_read_byte();
	therm_reset();
	*digit = firstbyte>>4;
	*digit|= (secondbyte&0x7)<<4;
	*decimal =firstbyte&0x0F;
	*decimal *=THERM_DECIMAL_STEPS_12BIT;

	//sprintf(buffer, "%+d.%04uC", digit, decimal)

}

void therm_read_temperature(void){

	int digit=0, decimal=0;

	therm_read_temperatureRAW(&digit,&decimal);
	lcd_putint(digit);
	lcd_putchar('.');
	lcd_putint(decimal/100);
}


