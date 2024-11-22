// uart.c

#include <avr/io.h>
#include <util/delay.h>
#include <compat/deprecated.h>
#include "uart.h"



void uart_init()
{
	UBRR0H = 0x00; UBRR0L = 0x07;	// 57.6Kbps
	sbi(UCSR0A, U2X0);				// 115.2Kbps, *2 baud rate
	sbi(UCSR0B, TXEN0);				// TX enable
}

void uart_putchar(char ch)
{
	if (ch == '\n' )
		uart_putchar( '\r' );

	_delay_ms(10);
	UDR0 = ch;
}

void uart_putstr(char *sp)
{
	for( ; *sp; sp++)
		uart_putchar(*sp);
}
