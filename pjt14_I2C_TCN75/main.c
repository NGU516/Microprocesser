// main.c

#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "uart.h"
#include "linked_list.h"
#include "Timer_linked_list.h"
#include "app.h"
#include "Task_queue.h"
#include "Timer.h"
#include "spi.h"

int main()
{
	int tag;
	struct task task;

	uart_init();
	task_init();
	timer_init();
	spi_init();

	sei();

	printf("$ ");

	while(1) {
		cli();
		tag = task_delete(&task);
		sei();
		if (tag)
			(*(task.fun))(task.arg);
	}
	return 0;
}
