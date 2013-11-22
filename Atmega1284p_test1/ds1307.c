/*
 * ds1307.c
 *
 * Created: 11/10/2013 1:33:35 PM
 *  Author: ryder
 */ 

#include "ds1307.h"
#include "glcdcontrols.h"
#define ENABLE_DEBUG 0

void I2C_init(){
	TWBR = ((F_CPU/F_SCL)-16)/2;
	TWSR = 0;
	TWCR = (1<<TWEN);
}

//Return status codes if something wrong, otherwise return 0
byte I2C_start() {
	if(ENABLE_DEBUG) show_message(0,0, "start    ");
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	
	while(!(TWCR & (1<<TWINT))); //Wait till it finishes
	if(ENABLE_DEBUG) show_message(0,0, "done start");
	if(((TWSR & 0xF8) != 0x08) && ((TWSR & 0xF8) != 0x10)){
		return TWSR;
	} else {
		return 0;
	}
}

byte I2C_send_address(byte address){
	byte status = 0;
	TWDR = address;
	if(ENABLE_DEBUG) show_message(0,0, "send address  ");
	TWCR = (1<<TWINT)|(1<<TWEN);	
	while(!(TWCR & (1<<TWINT)));
	if(ENABLE_DEBUG) show_message(0,0, "done address");
	status = (TWSR & 0xF8);
	if((status != 0x18) && (status !=0x40)){//0x18 is for SLA+W and 0x40 is for SLA+R (Refer atmega1284p datasheet p223)
		return TWSR;
	} else {
		return 0;
	}
}

byte I2C_write(byte data){
	TWDR = data;
	if(ENABLE_DEBUG) show_message(0,0, "writing");
	TWCR |= (1<<TWINT)|(1<<TWEN);	
	while(!(TWCR & (1<<TWINT)));
	if(ENABLE_DEBUG) show_message(0,0, "done writing  ");
	if((TWSR & 0xF8) != 0x28){
		return TWSR;
	} else {
		return 0;
	}
	
}

void I2C_stop(){ //Instead of a function, can use a #define macro as well since we're not waiting for it to return anything
	if(ENABLE_DEBUG) show_message(0,0, "stop     ");
	TWCR |= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(TWCR & (1<<TWSTO));
}

byte I2C_read_nack(){	
	if(ENABLE_DEBUG) show_message(0,0, "read       ");
	TWCR = (1<<TWINT)|(1<<TWEN);//NACK = Don't read anymore data
	if(ENABLE_DEBUG) show_message(0,0, "read2       ");
	_delay_ms(10);
	while(!(TWCR & (1<<TWINT)));
	if(ENABLE_DEBUG) show_message(0,0, "done read       ");
	return TWDR;	
}

void I2C_write_register(byte device_register, byte data){
	I2C_start();
	I2C_send_address(DS1307);
	I2C_write(device_register);
	I2C_write(data);
	I2C_stop();
}

byte I2C_read_register(byte device_register){
	byte data = 0;
	I2C_start();
	I2C_send_address(DS1307);
	I2C_write(device_register);
	I2C_start();
	I2C_send_address(DS1307+1);//Read operation
	data = I2C_read_nack();
	I2C_stop();
	return data;
}

void get_time(byte *hours, byte *minutes, byte *seconds){ // returns hours, minutes, and seconds in BCD format
	*hours = I2C_read_register(HOURS_REGISTER);
	*minutes = I2C_read_register(MINUTES_REGISTER);
	*seconds = I2C_read_register(SECONDS_REGISTER);
	if (*hours & 0x40) // 12hr mode:
	*hours &= 0x1F; // use bottom 5 bits (pm bit = temp & 0x20)
	else
	*hours &= 0x3F; // 24hr mode: use bottom 6 bits
}

void get_date(byte *months, byte *days, byte *years){ // returns months, days, and years in BCD format
	*months = I2C_read_register(MONTHS_REGISTER);
	*days = I2C_read_register(DAYS_REGISTER);
	*years = I2C_read_register(YEARS_REGISTER);
}

void set_time_date() // simple, hard - coded way to set the date 8/13/21013 at 8:51 PM
{
	I2C_write_register(MONTHS_REGISTER,0x0B);
	I2C_write_register(DAYS_REGISTER,0x21);
	I2C_write_register(YEARS_REGISTER, 0x10);
	I2C_write_register(HOURS_REGISTER, 0x08); // add 0x40 for PM
	I2C_write_register(MINUTES_REGISTER, 0x27);
	I2C_write_register(SECONDS_REGISTER, 0x00);
}

byte bcd_decode(byte data){
	int temp = data >> 4;
	data &= 0x0F;
	return (data + temp*10);
}

void I2C_enable_square_wave(){
	I2C_start();
	I2C_send_address(DS1307);
	I2C_write(0x07);
	I2C_write(0b00010000);
	I2C_stop();
}