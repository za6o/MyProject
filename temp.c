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
#include <stdbool.h>

#include "lcd.h"
#include "defines.h"


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

	uint8_t firstbyte;
	uint8_t secondbyte;



#ifdef bit11
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_WSCRATCHPAD);
	therm_write_byte(0x00);
	therm_write_byte(0x00);
	therm_write_byte(0x5f); // 11 bit resolution

	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_CPYSCRATCHPAD);
	while(!therm_read_bit());//_delay_ms(20);

#endif
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_CONVERTTEMP);
	while(!therm_read_bit());

	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_RSCRATCHPAD);
	firstbyte=therm_read_byte();
	secondbyte=therm_read_byte();
	therm_reset(); // try to remove, it should work without this reset

#ifdef bit11
	*digit = firstbyte>>4;
	*digit|= (secondbyte&0x7)<<4;
	*decimal =(firstbyte>>1)&0x07;
	*decimal *=THERM_DECIMAL_STEPS_11BIT;

#else
	*digit = firstbyte>>4;
	*digit|= (secondbyte&0x7)<<4;
	*decimal =firstbyte&0x0F;
	*decimal *=THERM_DECIMAL_STEPS_12BIT;
#endif

}

void display_temp(void){

	int digit=0, decimal=0;
	//static bool lock = false;
	//		if (!lock){
	//			lock = true;
				therm_read_temperatureRAW(&digit,&decimal);
				lcd_putint(digit);
				lcd_putchar('.');
				lcd_putint(decimal/10);
	//			lock = false;
	//		}
}




