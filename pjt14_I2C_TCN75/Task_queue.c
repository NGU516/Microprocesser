// Task_queue.c

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "Task_queue.h"
#include "app.h"
#include "Timer_linked_list.h"
#include "adc.h"
#include "spi.h"
#include "i2c.h"

void task_init()
{
	Task_f = Task_r = 0;
}

int task_insert(struct task *tskp)
{
	if ((Task_r + 1) % MAX_TASK == Task_f)	// Full
		return 0;
	Task_r = (Task_f + 1) % MAX_TASK;
	Task_q[Task_r] = *tskp;
	return 1;
}
int task_delete(struct task *tskp)
{
	if (Task_r == Task_f)	// Empty
		return 0;
	Task_f = (Task_f + 1) % MAX_TASK;
	*tskp = Task_q[Task_f];
	return 1;
}

// Analog temperature data
void task_tc1047(char *arg)
{
	static int value;
	static int volatile in_adc = 0;
	if (!strcmp(arg, "")) {		// called from task_cmd or timer task
		adc_start(); in_adc = 1; while(in_adc);
		printf("Current temperature is %d degree.\n", value);
	}
	else {		// called from ISR()
		value = atoi(arg) * (1.1/1023) * 1000;	// get input voltage(mv)
		value = (value - 500) / 10;				// get degree from input voltage
		in_adc = 0;
	}
}

// SPI temperature data
void task_tc77(char *arg)
{
	static uint8_t state;
	static uint8_t value;

	if (!strcmp(arg, "")) {
		state = TC77_WAIT_HI;
		spi_select();
		spi_write(0x00);
		printf("interrupt flag");
	}
	else {
		switch(state) {
			case TC77_WAIT_HI :
				value = atoi(arg) << 8;	state = TC77_WAIT_LO; spi_write(0x00);
				break;
			case TC77_WAIT_LO :
				value |= atoi(arg);	value = (value >> 3) * 0.0625;
				spi_release();
				printf("task_tc77() : current_temperature -> %d degree.\n", value);
				break;
			default:
				spi_release();
				printf("task_tc77() : unexpected state in task_tc77...\n");
		}
	}
}

// TCN75 temperature data
void task_tcn75_i2c(void *arg)
{
	uint16_t value;
	i2c_tcn75_trans_start();	// communicate start
	// address + write_operation
	if (i2c_tcn75_write_one_byte(0x90) != 0) {
		i2c_tcn75_trans_stop();	printf("task_tcn75_i2c() : SLA+W write fail...\n");
		return;
	}
	// pointer(TEMP)
	if (i2c_tcn75_write_one_byte(0x00) != 0) {
		i2c_tcn75_trans_stop(); printf("task_tcn75_i2c() : pointer write fail...\n");
		return;
	}
	i2c_tcn75_trans_start();
	if (i2c_tcn75_write_one_byte(0x91) != 0) {
		i2c_tcn75_trans_stop(); printf("task_tcn75_i2c() : SLA+W write fail...\n");
		return;
	}
	value = ((i2c_tcn75_read_one_byte(0) << 8) | i2c_tcn75_read_one_byte(1)) >> 7;
	i2c_tcn75_trans_stop();

	value = value >> 1;
	printf("task_tcn75_i2c() : current_temperature -> %d degree.\n", value);
}

void task_cmd(char *arg)
{
	char buf[64], *cp0, *cp1, *cp2, *cp3;
	struct task task;
	int ms;

	if (!uart_peek()) return;

	if (gets(buf) == NULL) {
		printf("logout, good bye !!!\n");
		while(1);
	}

	cp0 = strtok(buf, " \t\n\r");
	cp1 = strtok(NULL, " \t\n\r");
	cp2 = strtok(NULL, " \t\n\r");
	cp3 = strtok(NULL, " \t\n\r");

	if (cp0 == NULL) {
		// printf("!!!-111\n");
		printf( "$ "); return;
	}

	if (!strcmp(cp0, "prime"))
		task_prime(cp1);

	else if (!strcmp(cp0, "timer")) {
		if (cp1 == NULL) {
			printf("!!!-222\n");
			printf("$ ");
			return;
		}
		ms = atoi(cp1)/256;
		if(!strcmp(cp2, "prime")) {
			task.fun = task_prime;
			if(cp3) strcpy(task.arg, cp3);
			else strcpy(task.arg, "");
			cli();
			insert_timer(&task, ms);
			sei();
		}
		else printf("!!!-333\n");
	}

	else if (!strcmp(cp0, "tc1047"))
		task_tc1047("");

	else if (!strcmp(cp0, "tc77"))
		task_tc77("");
	else if (!strcmp(cp0, "tcn75_i2c"))
		task_tcn75_i2c("");

	else printf("Unknown command...\n");

	printf("$ ");
}
