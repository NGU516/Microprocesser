// main.c

#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

int main()
{
	led_init();

	while(1)
	{
		led_off_all();
		
		for(uint8_t i=0; i<8; i++){
			led_off(i-1);
			led_on(i);
			_delay_ms(300);
		}

		for(uint8_t i=7; i>0; i--){
			led_on(i);
			led_off(i+1);
			_delay_ms(300);
		}
		
		
	}

	return 0;
}
