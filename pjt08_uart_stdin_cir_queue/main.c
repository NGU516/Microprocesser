// main.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "uart.h"
#include "uart_q.h"
#include "app.h"

int main()
{	
	char cmd[128], *cp, *cp2, *cp3;

	uart_init();
	led_init();

	sei();

	while(1) {
		printf("$ ");
		if (fgets(cmd, sizeof(cmd), stdin) == NULL)
			break;
		if ((cp = strtok(cmd,  "\n\r\t  ")) == NULL) continue;
		cp2 = strtok(NULL, "\n\r\t    ");
		cp3 = strtok(NULL, "\n\r\t    ");
		// printf("%c %c %c", *cp, *cp2, *cp3);
		printf("%s\n", cp);

		if (!strcmp(cp, "prime")) app_prime(2000);
		else if(!strcmp(cp, "led") && cp2 == NULL) led_status_hex();
		else if(!strcmp(cp, "led") && !strcmp(cp2, "on") && !strcmp(cp3, "all"))
			led_on_all();
		else if(!strcmp(cp, "led") && !strcmp(cp2, "off") && !strcmp(cp3, "all"))
			led_off_all();
		else if(!strcmp(cp, "led") && !strcmp(cp2, "on")) {
			int temp = atoi(cp3);
			led_on(temp);
		}
		else if(!strcmp(cp, "led") && !strcmp(cp2, "off")) {
			int temp = atoi(cp3);
			led_off(temp);
		}

		else 					  printf( "Unknown command... \n");
	}
	printf("logout, good bye !!!\n");
	while(1); return 0;

}
