// Timer.c

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "Timer_linked_list.h"
#include "Task_queue.h"

void timer_init()
{
	TCNT2 = 0;					// Timer/Counter Register of Timer 2
	ASSR |= (1 << AS2);			// Asynchronous Status Register(Asynchronous Timer/Counter 2
	TIMSK2 |= (1 << TOIE2);		// Timer/Counter Interrupt Mask Register of timer 2
	// Timer/Counter Control Register B of timer 2
	TCCR2B |= (1 << CS20);	TCCR2B |= (1 << CS21);	// 32KHz/32 prescaling, clock 1KHz
}

void timer_expire()
{
	struct timer *tp;

	for( ; Thead != NULL && !Thead->time; ) {
		tp = Thead, Thead = tp->link;

		task_insert(&tp->task);

		free(tp);
	}
}

////////////////////////////////
ISR(TIMER2_OVF_vect)
{
	if (!Thead)
		return;
	if (--Thead->time == 0)
		timer_expire();
}
