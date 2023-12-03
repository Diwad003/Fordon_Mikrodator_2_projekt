#include "global_defs.h"
#include "usart.h"
#include "Timers.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init();
void command_handler();
void ADC_INIT();

#define Buffer_size_usart_receive 8
volatile uint8_t usart_receive[Buffer_size_usart_receive];
volatile uint8_t usart_index;
volatile uint8_t string_done;

volatile uint16_t sensor_data;
volatile uint16_t sensor_data_cm;

int main(void)
{
	init();
	while (1)
	{
		if (string_done == 1)
		{
			command_handler();
		}
	}
}

void init()
{
	USART_INIT();
	ADC_INIT();
	Set_Timer2(255);
	//Set_Timer1(1, 1, 1, 0, 30);//fram	
	//Set_Timer3(1, 0, 1, 1, 30);//bak
	
	sei();
	printf("Init_done\n\r");
}

void command_handler()
{
	switch (usart_receive[4])
	{
		case 'J':
		if(usart_receive[0] == 'F')
		{
			Set_Timer1(1, 1, 1, 0, usart_receive[2]);
		}
		else if (usart_receive[0] == 'B')
		{
			Set_Timer1(1, 0, 1, 1, usart_receive[2]);
		}
		else if(usart_receive[0] == 'S')
		{
			Set_Timer1(0, 0, 0, 0, 0);
		}
		
		if(usart_receive[1] == 'F')
		{
			Set_Timer3(1, 1, 1, 0, usart_receive[3]);
		}
		else if (usart_receive[1] == 'B')
		{
			Set_Timer3(1, 0, 1, 1, usart_receive[3]);
		}
		else if(usart_receive[1] == 'S')
		{
			Set_Timer3(0, 0, 0, 0, 0);
		}
		
		for (int i = 0; i<sizeof(usart_receive); i++)
		{
			usart_receive[i] = 0;
		}
		string_done = 0;
		
		break;
		
		case 'S':
		break;
		
		default:
		break;
	}
}

void ADC_INIT(void)
{
	ADCSRA = (1<<ADEN) | (1<<ADIE);
}

ISR(ADC_vect)
{
	uint16_t adc_out = ADC;
	if (adc_out != sensor_data)
	{
		sensor_data = adc_out;
		sensor_data_cm = ((6787)/(sensor_data-3))-4;
		USART0_TX_STRING((char*)sensor_data_cm);
	}
}


ISR(TIMER2_COMPA_vect)
{
	ADCSRA |= (1<<ADSC);
}

ISR(USART0_RX_vect)
{
	char data = USART0_RX();
	if (data == '&')
	{
		usart_index = 0;
		string_done = 1;
	}
	else
	{
		usart_receive[usart_index] = data;
		usart_index++;
	}
}