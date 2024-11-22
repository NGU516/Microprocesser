// uart.c

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "uart_q.h"

FILE Mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE Mystdin = FDEV_SETUP_STREAM (NULL, uart_getchar, _FDEV_SETUP_READ);
char uart_busy;

void uart_init()
{
	stdin = &Mystdin; stdout = &Mystdout;
	q_init();
	uart_busy = 0;

	UBRR0H = 0x00; UBRR0L = 0x03;	// 115.2Kbps
	sbi(UCSR0B, TXEN0);				// TX enable
	sbi(UCSR0B, TXCIE0);			// TX complete interrupt
	sbi(UCSR0B, RXEN0);				// RX enable
	sbi(UCSR0B, RXCIE0);			// RX complete interrupt
}

int uart_putchar(char ch, FILE *stream)
{
	if (ch == '\n') uart_putchar('\r', stream);
	cli();
	if(!uart_busy) {
		UDR0 = ch;
		uart_busy = 1;
	}
	else {
		while(qo_insert(ch) == 0) {
			sei();
			_delay_us(100);
			cli();
		}
	}
	sei();
	return 1;
}

int uart_getchar(FILE *stream)
{
	char ch;

	do {
		cli();
		ch = qi_delete();
		
		sei();
	} while (ch == 0);

	if (ch == E0T) return -1;
	else 		   return ch;
}

void uart_echo(char ch)
{
	if (ch == '\n')	uart_echo( '\r' );
	if (!uart_busy) {
		UDR0 = ch;
		uart_busy = 1;
	}
	else
		qo_insert(ch);

}

////////////////////////////////////////////
ISR(USART0_TX_vect)
{
	char ch;
	if ((ch = qo_delete()) == 0)
		uart_busy = 0;
	else
		UDR0 = ch;
	
		
}

ISR(USART0_RX_vect)
{
	char ch;
	ch = UDR0;
	if (ch != E0T) {
		if (ch == '\r') ch = '\n';
		uart_echo(ch);
	}
	if(ch == 0x08){
		return;
	}
	else		
		qi_insert(ch);
}
