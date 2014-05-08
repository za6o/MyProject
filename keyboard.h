

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

void init_keyboard(void);
uint8_t ParseKey(uint16_t,uint8_t*);
bool GetKey(uint8_t*);

#endif
