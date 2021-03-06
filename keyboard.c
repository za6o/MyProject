
#include <stdio.h>
#include <stdbool.h>

#include "adc.h"
#include "defines.h"
#include "keyboard.h"



#include <util/delay.h>



static bool EndLine = false;

void init_keyboard(){
	init_adc();
}

uint8_t ParseKey(uint16_t val, uint8_t* KeyPressed){

	val=1023-val;

	if((30<=val)&&(val<=70)) {*KeyPressed=1; return 1;}
	else if((71<=val)&&(val<=110)) {*KeyPressed=2;return 1;}
	else if((111<=val)&&(val<=164)) {*KeyPressed=3;return 1;}
	else if((165<=val)&&(val<=253)) {*KeyPressed=4;return 1;}
	else if((254<=val)&&(val<=360)) {*KeyPressed=5;return 1;}
	else if((361<=val)&&(val<=474)) {*KeyPressed=6;return 1;}
	else if((475<=val)&&(val<=611)) {*KeyPressed=7;return 1;}
	else if((612<=val)&&(val<=730)) {*KeyPressed=8;return 1;}
	else if((731<=val)&&(val<=808)) {*KeyPressed=9;return 1;}
	else if((877<=val)&&(val<=919)) {*KeyPressed=0;return 1;}
	else if((920<=val)&&(val<=1015)) {EndLine=true;return 1;}
	else if((809<=val)&&(val<=876)) {*KeyPressed=0xF0;return 1;}

	return 0;


}

bool GetKey(uint8_t* PressedKey){

	uint16_t adc=0;
	uint8_t prevKey=0xFF;
	bool gotKey = false;

	while(!(gotKey)){
		do{
			adc = read_adc(KEYBOARD);
		} while (ParseKey(adc, PressedKey) == 0);

		_delay_ms(50);

		if ((prevKey == *PressedKey)||(EndLine))
			gotKey=true;
		else
			prevKey = *PressedKey;
	}

	if (EndLine){
		EndLine=false;
		return false;
	}
	else
		return true;

}

