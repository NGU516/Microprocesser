// main.c

#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "uart.h"

int main()
{	
	char c;

	uart_init();	

	for(int i=0; i < 20; i++)
	{
		
		for (c = 'A'; c <= 'Z'; c++) 
			uart_putchar(c);
		uart_putchar('\n');
		
	}
	uart_putstr("The End");
	while(1);

	return 0;
}
