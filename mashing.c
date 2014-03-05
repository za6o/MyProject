
#include <avr/io.h>	// deal with port registers
#include <util/delay.h>	// used for _delay_us function
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd.h"
#include "temp.h"
#include "heat.h"


inline void	remaining_sec(uint16_t time, uint16_t desired_time)
{
	uint16_t diff=desired_time-time;
	if (diff <= 99){
		lcd_pos(2,15);
		lcd_putchar(' ');
	}
	if (diff <= 9){
		lcd_pos(2,14);
		lcd_putchar(' ');
	}

    lcd_pos(1,13);
    lcd_putint(diff);

}


inline uint8_t compare(uint8_t targT){

	int digit=0, decimal=0;

	therm_read_temperatureRAW(&digit,&decimal);
	lcd_putint(digit);
	lcd_putchar('.');
	lcd_putint(decimal/100);

	if ((int)targT <= digit)
		return 1;
	else
		return 0;
}

inline uint8_t step_mashing(uint8_t target_temp){

		lcd_pos(1,5);
		if(compare(target_temp)){
			stop_heating();
			return 1;
		}
		else
			start_heating();

		return 0;
}

uint8_t reaching_targ(uint8_t targ_temp)
{
	return (step_mashing(targ_temp));
}

uint8_t wait_time (uint8_t target_temp, uint16_t desired_time,uint16_t sec)
{
	step_mashing(target_temp);

	if(sec > desired_time){
		return 1;
	}
	else{
		remaining_sec(sec, desired_time);
		return 0;
	}

}

