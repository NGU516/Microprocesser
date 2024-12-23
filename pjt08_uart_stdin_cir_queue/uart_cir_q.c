// uart_cir_q.c

#include "uart_cir_q.h"

char qi[QI_SIZE], qo[QO_SIZE];
int fi, fo, ri, ro;

void q_init()
{
	fi = fo = ri = ro = 0;
}

int qi_insert(char ch)
{	
	if ((ri + 1) % QI_SIZE == fi)
		return 0;				// full
	ri = (ri + 1) % QI_SIZE;	// move
	qi[ri] = ch;				// data enqueue
	return 1; 		
}

int qi_delete()
{
	if (ri == fi)
		return 0;				// empty
	fi = (fi + 1) % QI_SIZE;	// move
	return qi[fi];				// data dequeue
}

int qo_insert(char ch)
{
	if ((ro + 1) % QO_SIZE == fo)
		return 0;
	ro = (ro + 1) % QO_SIZE;
	qo[ro] = ch;
	return 1;
}

int qo_delete()
{
	if (ro == fo)
		return 0;
	fo = (fo + 1) % QO_SIZE;
	return qo[fo];
}
