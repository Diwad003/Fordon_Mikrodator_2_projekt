#include "global_defs.h"
#include "usart.h"
#include "Timers.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/atomic.h>

void process_commands(char* buffer, uint8_t *direction0, uint8_t* direction1, uint16_t *velocityX, uint16_t *velocityY, uint8_t *honk);
void command_handler(uint8_t direction0, uint8_t direction1, uint16_t velocityX, uint16_t velocityY, uint8_t honk);
void ADC_INIT();
void destroyBuffer(char* buffer);

#define Buffer_size_usart_receive 16
volatile uint8_t usart_receive[Buffer_size_usart_receive];
volatile uint8_t usart_index;
volatile uint8_t string_done;
volatile uint8_t command_timeout;

volatile uint16_t sensor_data_cm = 0;
volatile uint8_t Timer_send_sensor = 0;
volatile uint8_t old_adc_value = -1;

int main(void)
{
	USART_INIT();
	ADC_INIT();
	Set_Timer2(150);
	SETBIT(DDRB, 0);
	FLIPBIT(DDRB, 1);

	sei();

	while (1)
	{
		if (string_done == 1)
		{
			ATOMIC_BLOCK(ATOMIC_FORCEON)
			{
				string_done = 0;
				uint8_t direction0 = 0;
				uint16_t velocityX = 0;
				uint16_t velocityY = 0;
				uint8_t direction1 = 0;
				uint8_t honk = 0;


				process_commands((char*)usart_receive, &direction0, &direction1, &velocityX, &velocityY, &honk);
				command_handler(direction0, direction1, velocityX, velocityY, honk);
			}
		}
		else if(command_timeout >= 150)
		{
			Set_Timer1(0, 0, 0, 0, 0);
			Set_Timer3(0, 0, 0, 0, 0);
			command_timeout = 0;
		}
		else if(Timer_send_sensor >= 255)
		{
			char str[16];
			sprintf(str, "%d&", sensor_data_cm);
			USART0_TX_STRING(str);
			Timer_send_sensor = 0;
		}
		else if(sensor_data_cm < 10)
		{
			SETBIT(PORTB, 0);
		}
		else
		{
			CLEARBIT(PORTB, 0);
		}
	}
}

void process_commands(char* buffer, uint8_t *direction0, uint8_t* direction1, uint16_t *velocityX, uint16_t *velocityY, uint8_t *honk)
{
	char* string_buffer[16];
	int8_t i = 0;
	
	string_buffer[i] = strtok(buffer, ":");
	while(string_buffer[i] != '\0')
	{
		i++;
		string_buffer[i] = strtok(NULL,":");
	}
	*direction0 = string_buffer[0][0];
	if (*direction0 != 'S')
	{
		*direction1 = string_buffer[1][0];
		*velocityX = atoi(string_buffer[2]);
		*velocityY = atoi(string_buffer[3]);
		*honk = atoi(string_buffer[4]);
	}
	else
	{
		*direction1 = 0;
		*velocityX = 0;
		*velocityY = 0;
		*honk = atoi(string_buffer[1]);
	}


	destroyBuffer(buffer);
}

void command_handler(uint8_t direction0, uint8_t direction1, uint16_t velocityX, uint16_t velocityY, uint8_t honk)
{
	if(direction0 == 'F')
	{
		if (velocityX > 500)
		{
			Set_Timer1(1, 1, 1, 0, velocityX);
		}
		else
		{
			Set_Timer1(1, 0, 1, 1, velocityX);
		}
	}
	else if (direction0 == 'B')
	{
		if (velocityX > 500)
		{
			Set_Timer1(1, 0, 1, 1, velocityX);
		}
		else
		{
			Set_Timer1(1, 1, 1, 0, velocityX);
		}
	}
	else if(direction0 == 'S')
	{
		Set_Timer1(0, 0, 0, 0, 0);
		Set_Timer3(0, 0, 0, 0, 0);
	}
	
	if(direction1 == 'F')
	{
		if (velocityY > 500)
		{
			Set_Timer3(1, 1, 1, 0, velocityY);
		}
		else
		{
			Set_Timer3(1, 0, 1, 1, velocityY);
		}
	}
	else if (direction1 == 'B')
	{
		if (velocityY > 500)
		{
			Set_Timer3(1, 0, 1, 1, velocityY);
		}
		else
		{
			Set_Timer3(1, 1, 1, 0, velocityY);
		}
	}
	
	if(honk == 1)
	{
		Set_Timer0(1, 250);
	}
	else
	{
		Set_Timer0(0, 0);
	}
}

void ADC_INIT(void)
{
	ADMUX = 0;
	ADCSRA = (1<<ADEN) | (1<<ADIE);
}

void destroyBuffer(char* buffer)
{
	for (int i = 0; i<sizeof(buffer); i++)
	{
		buffer[i] = 0;
	}
}

ISR(ADC_vect)
{
	uint16_t adc_out = ADC;
	if(old_adc_value != adc_out)
	{
		sensor_data_cm = ((6787)/(adc_out-3))-4;
		old_adc_value = adc_out;
	}
	Timer_send_sensor++;

	ADCSRA |= (1<<ADSC);
}

ISR(TIMER0_COMPA_vect)
{
	FLIPBIT(PORTB, 1);
}

ISR(TIMER2_COMPA_vect)
{
	command_timeout++;
	ADCSRA |= (1<<ADEN) | (1<<ADSC);
}

ISR(USART0_RX_vect)
{
	command_timeout = 0;
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