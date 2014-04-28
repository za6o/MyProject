

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "adc.h"
#include "defines.h"
#include "keyboard.h"



#include <util/delay.h>
#include "lcd.h"


bool EndLine = false;

uint8_t ParseKey(uint16_t val, uint8_t* KeyPressed){

	//static uint8_t KeyPressed;

	val=1023-val;
	if((45<=val)&&(val<=70)) {*KeyPressed=1; return 1;}
	else if((71<=val)&&(val<=110)) {*KeyPressed=2;return 1;}
	else if((111<=val)&&(val<=170)) {*KeyPressed=3;return 1;}
	else if((171<=val)&&(val<=250)) {*KeyPressed=4;return 1;}
	else if((251<=val)&&(val<=360)) {*KeyPressed=5;return 1;}
	else if((361<=val)&&(val<=480)) {*KeyPressed=6;return 1;}
	else if((481<=val)&&(val<=620)) {*KeyPressed=7;return 1;}
	else if((621<=val)&&(val<=710)) {*KeyPressed=8;return 1;}
	else if((711<=val)&&(val<=860)) {*KeyPressed=9;return 1;}
	else if((861<=val)&&(val<=909)) {*KeyPressed=0;return 1;}
	else if((910<=val)&&(val<=1015)) {EndLine=true;return 0;}


	return 0;
//	else if(830<=val<=860 ) {ParsedVal=*}

}

bool GetKey(uint8_t* PressedKey){

	uint16_t adc=0;


	while(ParseKey(adc, PressedKey) == 0){
		_delay_ms(80);
		ParseKey(adc, PressedKey);
		adc = read_adc(KEYBOARD);
		if (EndLine) return false;
	}

	return true;


}
