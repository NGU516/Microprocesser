// uart.c

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char buf[64];
int volatile bufi, txend;

void uart_init()
{
   UBRR0H = 0x00; 	UBRR0L = 0x03; // 115.2KBPS
   UCSR0B |= (1 << TXEN0); // TX enable
   UCSR0B |= (1 << TXCIE0); // TX complete interrupt enable
   // sbi(set bit), cbi(clear bit)
}

void uart_putstart(char str[])
{
	char ch;

	while(!txend); txend = 0;

	strcpy(buf, str);	bufi = 0;
	ch = buf[bufi++];

	UDR0 = ch;
}

///////////////////////////////////////
ISR(USART0_TX_vect)
{
	char ch = buf[bufi];
	if (!ch) { txend = 1; return; }
	if (ch == '\n') buf[bufi] = '\r';	// \n new line, \r carrage return
	else bufi++;

	UDR0 = ch;
}
