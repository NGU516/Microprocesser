// i2c.c

#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "i2c.h"

void tcn75_i2c_init()
{
	sbi(DDRD, SDA); sbi(PORTD, SDA);	// output, hi
	sbi(DDRD, SCL); sbi(PORTD, SCL);	// output, hi
	
	// address output
	// sbi(DDRH, 1); cbi(PORTH, 1);		// output, A0 <- 0
	// sbi(DDRH, 3); cbi(PORTH, 3);		// output, A1 <- 0	
	// sbi(DDRH, 5); cbi(PORTH, 5);		// output, A2 <- 0	
}

void i2c_tcn75_trans_start()
{
	cbi(PORTD, SCL);					// SCL <- Low
	sbi(PORTD, SDA);	delay_2nop();	// SDA <- High
	sbi(PORTD, SCL);	delay_2nop();	// SCA <- High
	cbi(PORTD, SDA);	delay_us(1);	// SDA <- Low
}

void i2c_tcn75_trans_stop()
{
	cbi(PORTD, SCL);					// SCL <- L
	cbi(PORTD, SDA);	delay_2nop();	// SDA <- L
	sbi(PORTD, SCL);	delay_2nop();	// SCL <- H
	sbi(PORTD, SDA);	delay_us(1);	// SDA <- H
}

uint8_t i2c_tcn75_write_one_byte(uint8_t data)
{
	uint8_t mask, ack;		// SDA output(Write)
	sbi(DDRD, SDA);			// SDA <- H
	for (mask = 0x80; mask; mask = mask >> 1) {
		cbi(PORTD, SCL);	delay_2nop();	// SCL <- L

		if (mask & data) sbi(PORTD, SDA);	// SDA <- 1
		else			 cbi(PORTD, SDA);	// SDA <- 0
		delay_us(1);
		sbi(PORTD, SCL); delay_us(1);		// SCL <- H
	}
	cbi(DDRD, SDA);			// SDA input(read)
	cbi(PORTD, SCL);	delay_us(1);	// SCL <- L
	sbi(PORTD, SCL);	delay_us(1);	// SCL <- H

	ack = PIND & (1 << SDA);		// ack <- SDA read ack of TC75 must be 0

	return ack;	
}

uint8_t i2c_tcn75_read_one_byte(uint8_t ack)
{
	uint8_t i, data = 0;
	cbi(DDRD, SDA);							// SDA input

	for (i = 0; i < 8; i++) {
		cbi(PORTD, SCL);	delay_us(1);	// SCL <- L
		sbi(PORTD, SCL);	delay_us(1);	// SCL <- H
		data = (data << 1);
		if (PIND & (1 << SDA)) data = data | 0x01;	// data <- SDA
	}

	sbi(DDRD, SDA);							// SDA output, for sending ack
	cbi(PORTD, SCL);	delay_2nop();		// SCL <- L

	if (ack) sbi(PORTD, SDA);				// SDA <- 0 or 1 (Ack or Nck)
	else 	 cbi(PORTD, SDA);
	delay_us(1);							// SCL <- H
	sbi(PORTD, SCL);	delay_us(1);

	return data;
}
