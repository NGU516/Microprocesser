// maic.c

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "led.h"
#include <avr/interrupt.h>

int main()
{
	int i;
	uart_init();
	led_init();
	led_off_all();
	sei();
	for(i=0; i < 10; i++) {
		uart_putstart("I love you.\n");
		uart_putstart("You love me.\n");
		uart_putstart("He loves her.\n");
	}
	uart_putstart("End !!!\n");
	while(1); return 0;
}
