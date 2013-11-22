/*
 * incfile.h
 *
 * Created: 11/10/2013 1:06:48 PM
 *  Author: ryder
 */ 


#ifndef INCFILE_H_
#define INCFILE_H_

#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define wait(x) _delay_ms(x)

#define setbit(x,y) x|=(1<<y)
#define clearbit(x,y) x&=~(1<<y)
#define togglebit(x,y) x^=(1<<y)

#define RS  PORTD0
#define RW  PORTD1
#define E   PORTD2

#define CS1 PORTD3
#define CS2 PORTD4
#define RST PORTD5

#define LED PORTD7

#define RSPORT PORTD
#define RWPORT PORTD
#define EPORT PORTD

#define CS1PORT PORTD
#define CS2PORT PORTD
#define RSTPORT PORTD

#define LEDPORT PORTD

#define DATA PORTA

#define F_SCL 200000L
#define DS1307 0xD0
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06

#define byte unsigned short

extern const char bart[1024];
extern const char eye[1024];
extern const char ic[1024];
extern const unsigned char font[760];
extern const unsigned short bigfont[];
extern const char byte_reverse[256];

#endif /* INCFILE_H_ */