// uart.h

#include <stdio.h>

extern FILE Mystdout;
extern char uart_busy;

extern void uart_init();
extern int uart_putchar(char ch, FILE *stream);
