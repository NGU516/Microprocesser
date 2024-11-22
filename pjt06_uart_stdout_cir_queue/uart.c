// uart.c

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "cir_queue.h"

FILE Mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);
char uart_busy;

void uart_init()
{
	stdout = &Mystdout;
	UBRR0H = 0x00; UBRR0L = 0x03;	// 115.2Kbps
	sbi(UCSR0B, TXEN0);				// TX enable
	sbi(UCSR0B, TXCIE0);
}

int uart_putchar(char ch, FILE *stream)
{
	if (ch == '\n')
		uart_putchar('\r', stream);

	cli();	// clear interrupt flag

	if (!uart_busy) {
		UDR0 = ch;
		uart_busy = 1;
	}
	else {
		while(q_insert(ch) == 0) {
			cli();
			_delay_us(100);
			sei();
		}
	}
	sei();	// set interrupt flag

	return 1;
}

////////////////////////////////////////////
ISR(USART0_TX_vect)
{
	char ch;

	if ((ch = q_delete()) == 0)
		uart_busy = 0;
	else
		UDR0 = ch;
}
