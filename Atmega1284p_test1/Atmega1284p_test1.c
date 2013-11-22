/*
* Atmega1284p_test1.c
*
* Created: 11/8/2013 11:47:30 PM
*  Author: ryder
*/

#include "incfile.h"
#include "glcdcontrols.h"
#include "ds1307.h"
#include "i2cmaster.h"

void blink(int num) {
	//setbit(DDRA, 0);	//Set LED for output
	for (int i = 0; i < num; i++) {
		setbit(LEDPORT, LED);
		wait(500);
		clearbit(LEDPORT, LED);
		wait(500);
	}
}


void get_time_string(char *str){
	
	char buffer[20];
	byte hour = 0, min=0, sec=0;
	get_time(&hour, &min, &sec);
	
	hour = bcd_decode(hour);
	min = bcd_decode(min);
	sec = bcd_decode(sec);
	if(hour<10){
		itoa(0, str, 10);
		itoa(hour,buffer,10);
		strcat(str, buffer);
		} else {
		itoa(hour,str,10);
		//strcat(str, buffer);
	}
	strcat(str, ":");
	if(min<10){
		strcat(str, "0");
		itoa(min, buffer,10);
		strcat(str, buffer);
		} else {
		itoa(min,buffer,10);
		strcat(str, buffer);
	}
	
	strcat(str, ":");
	if(sec<10){
		strcat(str, "0");
		itoa(sec, buffer,10);
		strcat(str, buffer);
		} else {
		itoa(sec,buffer,10);
		strcat(str, buffer);
	}
	
	
}


volatile unsigned int interrupt_counter=0;
volatile byte star_ypos = 0;
volatile byte star_dir = 0;
volatile byte show_pic = 0;


void enable_counter_interrupt(){
	DDRB &= ~(1<<DDB0); //Set as input
	PORTB |= (1<<PORTB0); //activate internal pullup
	
	PCICR |= (1<<PCIE1);	//Enable interrupt for the said pin
	PCMSK1 |= (1<<PCINT8); //Enable external interrupt on this pin
	
}

void init(){
	enable_counter_interrupt();
	glcd_init();
	wait(1000);
	glcd_on();
	is_inverted=0;
	glcd_clear();
	I2C_init();
	I2C_enable_square_wave();
	sei();
}

ISR (PCINT1_vect) {
	cli();
	interrupt_counter++;
	if(interrupt_counter >= 5000){
		interrupt_counter = 0;
	}
	if(interrupt_counter%10 ==0){
		togglebit(LEDPORT, LED);
	}
	
	if(interrupt_counter%60 == 0){
		show_pic = 1;
	}
	
	if(star_dir == 0){
		star_ypos+=4;
		if(star_ypos >= 118)
		star_dir = 1;
		} else {
		star_ypos-=4;
		if(star_ypos <= 4)
		star_dir = 0;
	}
	
	sei();
}


int main(){
	int star_ypos_local = 0, pic_counter=0;
	char time_str[16];
	
	init();
	
	while(1){
		show_message(0, 128-star_ypos_local, " ");
		show_message(7, star_ypos_local, " ");
		
		star_ypos_local = star_ypos;
		get_time_string(time_str);
		show_message(3, 32, time_str);
		
		show_message(0, 128-star_ypos_local, "*");
		show_message(7, star_ypos_local, "*");
		
		wait(50);
		if(show_pic){
			if(pic_counter == 0){
				draw_picture(bart);
				} else if(pic_counter ==1) {
				draw_picture(eye);
				} else {
				draw_picture(ic);
			}
			if(pic_counter >=2){
				pic_counter = 0;
				} else {
				pic_counter++;
			}
			
			wait(5000);
			glcd_clear();
			show_pic = 0;
		}
		
	}
	return 0;
}