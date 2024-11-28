// adc.c

#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "task_queue.h"

void adc_init()
{
	cbi(DDRF, PF0);			// input PF0(=ADC0)GPIO
	ADMUX = 0x80 | 0x00;	// Ref : 1.1v, Port : PF0
}

void adc_start()
{
	sbi(ADCSRA, ADEN);		// ADC enable
	_delay_us(120);			// warming up
	sbi(ADCSRA, ADIE);		// ADC interruput enable
	sbi(ADCSRA, ADSC);		// start conversion
}

////////////////////////////////////////////
ISR(ADC_vect)
{
	uint16_t data;
	char	 arg[8];

	data = ADCW & 0x03ff;
	cbi(ADCSRA, ADEN);		// ADC stop

	sprintf(arg, "%d", data);

	task_tc1047(arg);
}
