// uart.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"

/* original
char buf[64];
int volatile bufi, txend;
char flag = 0;
*/

char buf[64];
int volatile bufi, txend = 1;



void uart_init()
{
	// UBRR0H = 0x00;	UBRR0L = 0x03; // 115.2k
	UBRR0H = 0x00;	UBRR0L = 0x07;
	UCSR0A |= (1 << U2X0);
	UCSR0B |= (1 << TXEN0);		// TX enable
	UCSR0B |= (1 << TXCIE0);	// interrupt enable
}

/* original
void uart_putstart(char str[])
{
	led_on_all();
	if (!flag) {
		flag = 1;
		return;
	}

	else {	
		char ch;
		strcpy(buf, str); bufi = 0;
		ch = buf[bufi++];

		UDR0 = ch;
	}
}
*/

void uart_putstart(char str[])
{
	led_on_all();

	char ch;

	while(!txend); txend = 0;

	led_off_all();
	strcpy(buf, str); bufi = 0;
	ch = buf[bufi++];
	
	/*
	if ( buf[0] && (63 - strlen(buf) > strlen(str))) {
		strcat(buf, str);
		return;
	}
	*/
	UDR0 = ch;
}


//////////////////////////////////////////////
ISR(USART0_TX_vect)
{
	char ch = buf[bufi];
	if(!ch) {
		txend = 1; 
		return;
	}
	if(ch == '\n') buf[bufi] = '\r';
	else bufi++;
	UDR0 = ch;
}
