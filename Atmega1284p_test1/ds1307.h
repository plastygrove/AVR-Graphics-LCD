/*
 * ds1307.h
 *
 * Created: 11/10/2013 1:33:50 PM
 *  Author: ryder
 */ 


#ifndef DS1307_H_
#define DS1307_H_
#include "incfile.h"
#include <avr/io.h>

void I2C_init();
void I2C_repeat_start();
byte I2C_start();
byte I2C_send_address(byte address);
byte I2C_write(byte data);
void I2C_stop();
byte I2C_read_nack();
void I2C_write_register(byte device_register, byte data);
byte I2C_read_register(byte device_register);
void I2C_enable_square_wave();
void get_time(byte *hours, byte *minutes, byte *seconds);
void get_date(byte *months, byte *days, byte *years);
void set_time_date();
byte bcd_decode(byte data);



#endif /* DS1307_H_ */