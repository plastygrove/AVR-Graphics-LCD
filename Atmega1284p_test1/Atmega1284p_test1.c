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

void get_time_string2(char *str){
	
	char buffer[20];
	byte hour = 0, min=0, sec=0;
	char hr[2], mn[2], sc[2];
	get_time(&hour, &min, &sec);
	
	hour = bcd_decode(hour);
	min = bcd_decode(min);
	sec = bcd_decode(sec);
	
	itoa(hour, hr, 10);
	itoa(min, mn, 10);
	itoa(sec, sc, 10);
	
	str[0] = 'a';
	str[1] = 'b';
	str[2] = '\0';
	//if(hour<10){
	//str[0] = '0';
	//str[1] = hr[0];
	//} else {
	//str[0] = hr[0];
	//str[1] = hr[1];
	//}
	//
	//str[2] = ':';
	//
	//if(min<10){
	//str[3] = '0';
	//str[4] = mn[0];
	//} else {
	//str[3] = mn[0];
	//str[4] = mn[1];
	//}
	//
	//str[5] = ':';
	//
	//if(sec<10){
	//str[6] = '0';
	//str[7] = sc[0];
	//} else {
	//str[6] = sc[0];
	//str[7] = sc[1];
	//}
	//
	//str[8] = '\0';
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

int other_main(){
	char buffer[6];
	init();
	while(1){
		itoa(interrupt_counter, buffer, 10);
		show_message(3,32, buffer);
		
	}
}

void myitoa(int num, char *str, int dumb){
	int ones = 0, tens=0;
	ones = num%10;
	tens = num/10;
	str[0] = tens + '0';
	str[1] = ones + '0';
	str[2] = '\0';
}

void doSome(char *str){
	byte hour = 0, min=0, sec=0;
	char hr[4], mn[4], sc[4];
	get_time(&hour, &min, &sec);
	
	hour = bcd_decode(hour);
	min = bcd_decode(min);
	sec = bcd_decode(sec);
	
	myitoa(hour, hr, 10);
	myitoa(min, mn, 10);
	myitoa(sec, sc, 10);
	
	str[0] = hr[0];
	str[1] = hr[1];
	
	str[2] = ':';
	str[3] = mn[0];
	str[4] = mn[1];
	
	str[5] = ':';
	str[6] = sc[0];
	str[7] = sc[1];
	
	
	//if(hour<10){
	//sprintf(hr, "0%d", hour);
	//} else {
	//sprintf(hr, "%d", hour);
	//}
	//
	//if(min<10){
	//sprintf(mn, "0%d", min);
	//} else {
	//sprintf(mn, "%d", min);
	//}
	//
	//if(sec<10){
	//sprintf(sc, "0%d", sec);
	//} else {
	//sprintf(sc, "%d", sec)
	//}
	//sprintf(str, "%2d:%2d:%2d", hour, min, sec);
	
	//str[0] = 'a';
	//str[1] = 'b';
	//str[2] = '\0';
}


int main(){
	int i=0, star_ypos_local = 0, pic_counter=0;
	char time_str[16];
	
	init();
	
	//get_time_string(time_str);
	//show_message(0, 32, time_str);
	
	while(1){
		
		byte hour = 0, min=0, sec=0;
		char hr[4], mn[4], sc[4];
		////cli();
		//get_time(&hour, &min, &sec);
		////sei();
		//hour = bcd_decode(hour);
		//min = bcd_decode(min);
		//sec = bcd_decode(sec);
		//
		//myitoa(hour, hr, 10);
		//myitoa(min, mn, 10);
		//myitoa(sec, sc, 10);
		//
		//time_str[0] = hr[0];
		//time_str[1] = hr[1];
		//
		//time_str[2] = ':';
		//time_str[3] = mn[0];
		//time_str[4] = mn[1];
		//
		//time_str[5] = ':';
		//time_str[6] = sc[0];
		//time_str[7] = sc[1];
		//
		//time_str[8] = '\0';
		
		show_message(0, 128-star_ypos_local, " ");
		show_message(7, star_ypos_local, " ");
		
		star_ypos_local = star_ypos;
		get_time_string(time_str);
		//doSome(time_str);
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

//int main() {
//int i = 0, cursor_x = 4, cursor_y = 0; //Define the cursor positions
//char str[] = "Hello there! How are you doing?"; // multi-line text!";
//init();
//wait(1000); //Putting an initial delay just in case
//glcd_on();
//is_inverted = 1; //Uncomment this to invert the display
//clear();
////do_test();
////print_big_string(&cursor_x, &cursor_y, str);
//
//while (1) {
//clear();
//cursor_x = 3;
//cursor_y = 0;
//
//print_string(&cursor_x, &cursor_y, str); //Still some bugs in the way the text is displayed
//cursor_x = 0;
//cursor_y = 0;
//for (i = 0; i < 10; i++) {
//write_char(&cursor_x, &cursor_y, i + 48);
//blink(1);
//}
//
//draw_picture(eye);
//blink(4);
//draw_picture(bart);
//blink(4);
//draw_picture(ic);
//blink(4);
//}
//
//return 0;
//}


