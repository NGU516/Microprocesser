// main.c

#include <avr/interrupt.h>
#include "led.h"
#include "uart.h"
#include "cir_queue.h"
#include "app.h"

int main()
{	
	led_init();
	uart_init();
	q_init();

	sei();
	led_on(1);
	app_prime(2000);

	while(1);

	return 0;
}
