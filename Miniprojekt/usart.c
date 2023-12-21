#include "usart.h"


static FILE usart_stream = FDEV_SETUP_STREAM(USART1_TX, NULL, _FDEV_SETUP_WRITE);

void USART_INIT(void)
{
	CLEARBIT(DDRD, 0);
	SETBIT(DDRD, 1);
	UBRR0 = 12;
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
	UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00);


	SETBIT(DDRD, 3);
	UBRR1 = 12;
	UCSR1B = (1<<TXEN1);
	UCSR1C = (1<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10);
	
	stdout = &usart_stream;
}

static int USART1_TX(char data, FILE* stream)
{
	uint8_t timeout_timer = 0;
	while(!(UCSR1A & (1<<UDRE1)))
	{
		if(timeout_timer++ > 1000)
		{
			return -1;
		}
	}

	UDR1 = data;
	return 0;
}

uint8_t USART0_TX(char data)
{
	uint8_t timeout_timer = 0;
	while(!(UCSR0A & (1<<UDRE0)))
	{
		if(timeout_timer++ > 1000)
		{
			return -1;
		}
	}

	UDR0 = data;
	return 0;
}

void USART0_TX_STRING(char* data)
{
	for (int i = 0; i<strlen(data); i++)
	{
		USART0_TX(data[i]);
	}
}


uint8_t USART0_RX()
{
	uint8_t timeout_timer = 0;
	while (!(UCSR0A & (1<<RXC0)))
	{
		if(timeout_timer++ > 1000)
		{
			return -1;
		}
	}

	return UDR0;
}