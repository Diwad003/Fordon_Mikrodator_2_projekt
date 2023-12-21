#ifndef USART_H_
#define USART_H_

#include "global_defs.h"

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

void USART_INIT(void);
static int USART1_TX(char data, FILE* stream);
uint8_t USART0_TX(char data);
void USART0_TX_STRING(char* data);
uint8_t USART0_RX();

#endif /* USART_H_ */