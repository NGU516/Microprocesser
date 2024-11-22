// main.c

#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "uart.h"
#include "uart_q.h"
#include "app.h"

int main()
{	
	char cmd[128], ch;
	int n = 0;

	led_init();
	uart_init();

	sei();
	led_off_all();
	printf("$ ");
	/*
	printf("you are happy \n");
	printf("i am happy \n");
	while(1);
	*/

	while(1) {
		cli();
		ch = qi_delete();
		sei();
		if (ch) {

			led_on(0);
			_delay_ms(100);

			if (ch == '\r') continue;
			if (ch == '\n') {

				led_on(2);
				_delay_ms(100);
				
				cmd[n] = 0;
				printf("_____%s\n", cmd);

				if (!strcmp(cmd, "app"))	app_prime(2000);
				else 					 printf("Unknown command... \n");
				n = 0;
				printf("$ ");
			}
			else
				cmd[n++] = ch;
		}

		led_off_all();
	}

	return 0;
}
