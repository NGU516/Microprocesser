// main.c

#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"
#include "uart.h"

#include <stdio.h>
#include <math.h>

/* original
int main()
{
	int i;
	uart_init();
	led_init();
	sei();
	txend = 0;

	led_on_all();

	for(i=0; i < 10; i++) {
		uart_putstart("I love you.\n");
			while(!txend); txend = 0;
			led_off_all();
		uart_putstart("You love me.\n");
			while(!txend); txend = 0;
		uart_putstart("He loves her.\n");
			while(!txend);
	}
	uart_putstart("End !!!\n");
	while(1); return 0;
}
*/

int main()
{

	led_init();
	uart_init();

	sei();

	for(int i=0; i<10000; i++) {
		uart_putstart("i love you.\n");
	}
	uart_putstart("THE END !!\n");
	while(1);
	return 0;
}
