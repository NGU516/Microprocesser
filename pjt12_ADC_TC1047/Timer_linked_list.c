// Timer_linked_list.c

#include <stdio.h>
#include <stdlib.h>
#include "Timer_linked_list.h"

struct timer *Thead = NULL;

struct timer *get_timer()
{
	struct timer *tp;

	tp = (struct timer *)malloc(sizeof(*tp));
	return tp;
}

void insert_timer(struct task *tskp, int ms)
{
	int 		 total;
	struct timer *tp, *cp, *pp;	// timer, current, previous pointer

	tp = get_timer();
	tp->task = *tskp;	// task pointer
	tp->time = ms;

	if (Thead == NULL) {	// list empty
		Thead = tp, tp->link = NULL;
		return;
	}

	pp = NULL, total = 0;
	for (cp = Thead; cp; cp = cp->link) {
		total += cp->time;
		if (total >= ms)
			break;
		pp = cp;
	}
	if (pp == NULL) {	// First insert
		cp->time -= tp->time;
		tp->link = cp, Thead = tp;
	}
	else if (cp == NULL) {	// Last insert
		tp->time -= total;	// pp->time
		pp->link = tp;
		tp->link = NULL;
	}
	else {	// Middle insert
		total -= cp->time;	// just before
		tp->time -= tp->time;
		pp->link = tp;
		tp->link = cp;
	}
}

void tour_timer()
{
	struct timer *cp;
	int	   total = 0;

	printf("\n");
	for (cp = Thead; cp != NULL; cp = cp->link)	{
		total += cp->time;
		printf("-->%d(%d) ", cp->time, total);
	}
	printf("\n");
}

void free_timer()
{
	struct timer *cp;

	for ( ; Thead != NULL; ) {
		cp = Thead;
		Thead = cp->link;
		free(cp);
	}
}
