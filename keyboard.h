

#ifndef KEYBOARD_H
#define KEYBOARD_H

void init_keyboard(void);
uint8_t ParseKey(uint16_t,uint8_t*);
bool GetKey(uint8_t*);

#endif
