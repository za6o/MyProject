/*
 * temp.h
 *
 *  Created on: Feb 20, 2014
 *      Author: za6opz
 */


#ifndef TEMP_H
#define TEMP_H

uint8_t therm_reset();

void therm_read_temperatureRAW(int*, int*); // reading temo from sensor

void therm_write_bit(uint8_t);

uint8_t therm_read_bit(void);

uint8_t therm_read_byte(void);

void therm_write_byte(uint8_t);

#endif
