// spi.c

#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>
#include "timer_linked_list.h"
#include "task_queue.h"


void spi_init()
{
	// PORTB
	sbi(DDRB, 0);	// output, nSS,nCS pin
	sbi(DDRB, 1);	// output  SCK pin
	sbi(DDRB, 2); 	// output  MOSI
	cbi(DDRB, 3);	// input   MISO

	sbi(SPSR, SPI2X);	// double speed

	SPCR = ((1 << MSTR) | (1 << SPE) | (1 << SPIE));	// MASTER, MSB first, Fosc/4
}

void spi_select()
{
	cbi(PORTB, 0);	// cbi(PORTB, nSS)
}

void spi_release()
{
	sbi(PORTB, 0);	// sbi(PORTB, nSS)
}

void spi_write(uint8_t data)
{
	SPDR = data;
}


////////////////////////////////////
ISR(SPI_STC_vect)
{
	struct task task;
	uint8_t		data;

	data = SPDR;
	task.fun = task_tc77;
	sprintf(task.arg, "%d", data);
	task_insert(&task);
}

