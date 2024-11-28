// i2c.h

#define delay_2nop()	asm volatile("nop" ::); asm volatile("nop"::);
#define delay_us	_delay_us
#define SCL 0
#define SDA 1

void tcn75_i2c_init();
void i2c_tcn75_trans_start();
void i2c_tcn75_trans_stop();
uint8_t i2c_tcn75_write_one_byte(uint8_t data);
uint8_t i2c_tcn75_read_one_byte(uint8_t ack);
