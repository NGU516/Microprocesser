// uart.h

#include <stdio.h>

#define EOT	0x04
#define ESC 0x1B

extern FILE Mystdout;
extern FILE Mystdin;
extern char uart_busy;

extern void uart_init();
extern int uart_putchar(char ch, FILE *stream);
int uart_getchar(FILE *stream);
void uart_echo(char ch);
int uart_peek();
void task_cmd(char *arg);
