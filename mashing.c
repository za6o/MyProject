
#include <avr/io.h>	// deal with port registers
#include <util/delay.h>	// used for _delay_us function
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd.h"
#include "temp.h"
#include "heat.h"


static void delay_1s(void)
{
  uint8_t i;

  for (i = 0; i < 100; i++)
    _delay_ms(10);
}

uint8_t step_mashing(uint8_t target_temp){

		lcd_pos(1,5);
		if(compare(target_temp)){
			stop_heating();
			return 1;
		}
		else
			start_heating();

		delay_1s();
		return 0;
}

uint8_t wait_time (uint8_t target_temp, uint16_t desired_time,uint16_t sec)
{

	step_mashing(target_temp);

	if(sec > desired_time){
		return 1;
	}
	else
		return 0;
}

