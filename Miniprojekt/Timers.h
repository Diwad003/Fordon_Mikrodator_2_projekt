#ifndef TIMERS_H_
#define TIMERS_H_

#include "global_defs.h"
#include <avr/io.h>

void Set_Timer0(uint8_t interupt, uint8_t OCR);
void Set_Timer1(uint8_t COM1A1_value, uint8_t COM1A0_value, uint8_t COM1B1_value, uint8_t COM1B0_value, uint16_t OCR);
void Set_Timer2(uint8_t OCR);
void Set_Timer3(uint8_t COM3A1_value, uint8_t COM3A0_value, uint8_t COM3B0_value, uint8_t COM3B1_value, uint16_t OCR);


#endif /* TIMERS_H_ */