#include "Timers.h"

void Set_Timer1(uint8_t COM1A1_value, uint8_t COM1A0_value, uint8_t COM1B1_value, uint8_t COM1B0_value, uint8_t OCR)
{
	SETBIT(DDRD, 5);
	SETBIT(DDRD, 4);
	TCCR1A = (COM1A1_value<<COM1A1) | (COM1A0_value<<COM1A0) | (COM1B1_value<<COM1B1) | (COM1B0_value<<COM1B0) | (1<<WGM10);
	TCCR1B = (1<<CS02) | (1<<WGM12);
	OCR1A = OCR;
	OCR1B = OCR;
}

void Set_Timer2(uint8_t OCR)
{
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    OCR2A = OCR; 
    TIMSK2 = (1 << OCIE2A);
}

void Set_Timer3(uint8_t COM3A1_value, uint8_t COM3A0_value, uint8_t COM3B1_value, uint8_t COM3B0_value, uint8_t OCR)
{
	SETBIT(DDRB, 6);
	SETBIT(DDRB, 7);
	TCCR3A = (COM3A1_value<<COM3A1) | (COM3A0_value<<COM3A0) | (COM3B1_value<<COM3B1) | (COM3B0_value<<COM3B0) | (1<<WGM30);
	TCCR3B = (1<<CS02) | (1<<WGM32);
	OCR3A = OCR;
	OCR3B = OCR;
}