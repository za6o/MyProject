/*
 * adc.c

 *
 *  Created on: Feb 19, 2014
 *      Author: anaidenov
 */
#include <avr/io.h>
#include "adc.h"


void init_adc(void)
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
	ADMUX=(1<<REFS0)|(0<<REFS1);  // For Aref=AVcc;
//	ADMUX &= ~((1<<REFS0)|(1<<REFS1));   // For Aref=external;
	ADCSRA |= (1<<ADEN);                //Turn on ADC
	ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running

}

/*
 * read ADC in single conversation
 */

uint16_t read_adc(uint8_t channel)
{
	ADMUX &= 0xF0;                    //Clear the older channel that was read
	ADMUX |= channel;                //Defines the new ADC channel to be read
	ADCSRA |= (1<<ADSC);                //Starts a new conversion
	while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
	return ADCW;                    //Returns the ADC value of the chosen channe
}
