/*
* CFile1.c
*
* Created: 11/10/2013 1:09:54 PM
*  Author: ryder
*/

#include "incfile.h"
#include "glcdcontrols.h"

int is_inverted = 0;
void executeCommand() {
	//    wait(3);
	//    E = LOW; //Set Enable Low
	//    wait(3); //Wait for 2ms
	setbit(EPORT, E);//Set Enable High to start signal data transfer
	
	//    wait(1); //Wait for 2ms
	_delay_us(5);
	clearbit(EPORT, E); //Signal the end of data transfer
	//    wait(1); //Wait for 2ms
	_delay_us(5);
}

void glcd_on() {
	setbit(CS1PORT, CS1);
	setbit(CS2PORT, CS2);
	
	clearbit(RSPORT, RS);//Set RS Low
	clearbit(RWPORT, RW);//Set RW Low
	DATA = 0b00111111; //Display ON Instruction (0x3F)
	executeCommand();
}

void goto_yaddress(int yadd, int both_halves) {
	if (is_inverted) {
		if (both_halves) {
			setbit(CS1PORT, CS1);
			setbit(CS2PORT, CS2);
			if (yadd > 64) {
				DATA = 0b01000000 | (yadd - 64); //Here just in case
				} else {
				DATA = 0b01000000 | yadd;
			}
			} else {
			if (yadd < 64) {
				clearbit(CS1PORT, CS1);
				setbit(CS2PORT, CS2);
				DATA = 0b01000000 | (63 - yadd);
				} else {
				setbit(CS1PORT, CS1);
				clearbit(CS2PORT, CS2);
				DATA = 0b01000000 | (63 - yadd + 64);
			}
			clearbit(RSPORT, RS);
			clearbit(RWPORT, RW);
			
			clearbit(EPORT, E);
			
			executeCommand();
		}
		} else {
		if (both_halves) {
			setbit(CS1PORT, CS1);
			setbit(CS2PORT, CS2);
			if (yadd > 64) {
				DATA = 0b01000000 | (yadd - 64); //Here just in case
				} else {
				DATA = 0b01000000 | yadd;
			}
			} else {
			if (yadd < 64) {
				setbit(CS1PORT, CS1);
				clearbit(CS2PORT, CS2);
				DATA = 0b01000000 | (yadd);
				} else {
				clearbit(CS1PORT, CS1);
				setbit(CS2PORT, CS2);
				DATA = 0b01000000 | (yadd - 64);
			}
			clearbit(RSPORT, RS);
			clearbit(RWPORT, RW);
			
			clearbit(EPORT, E);
			
			executeCommand();
		}
	}
}

void goto_xpage(int xadd) {
	clearbit(RSPORT, RS);
	clearbit(RWPORT, RW);
	if (is_inverted) {
		DATA = 0b10111000 | (7 - xadd);
		} else {
		DATA = 0b10111000 | (xadd);
	}
	executeCommand();
}

void gotoxy(int x, int y) {

}

void write_data(short data) {
	if (is_inverted) {
		DATA = byte_reverse[data]; //Write 6 bits to screen
		} else {
		DATA = data;
	}
	setbit(RSPORT, RS);
	clearbit(RWPORT, RW);
	executeCommand();
}

void draw_picture(const char* pic) {
	int yadd = 0, xadd = 0, data = 0;
	for (xadd = 0; xadd < 8; xadd++) {
		for (yadd = 0; yadd < 128; yadd++) {
			data = pic[yadd + 128 * xadd];
			goto_yaddress(yadd, 0);
			goto_xpage(xadd);
			write_data(data);
		}
	}
}

void glcd_clear() {
	int yadd = 0, xadd = 0;
	for (xadd = 0; xadd < 8; xadd++) {
		goto_yaddress(0, 1);
		goto_xpage(xadd);
		write_data(0x00);
		for (yadd = 0; yadd < 64; yadd++) {
			executeCommand();
		}
	}
}

void write_char(int *x, int *y, unsigned char ch) {
	int i = 0, temp_y = *y;
	goto_yaddress(temp_y, 0);
	goto_xpage(*x); //Should set x address after setting y address
	for (i = 0; i < 8; i++) {
		goto_yaddress(temp_y, 0);
		write_data(font[(ch - 32)*8 + i]);
		temp_y += 1;
	}
}

void write_char2(int x, int y, unsigned char ch) {
	int i = 0, temp_y = y;
	goto_yaddress(temp_y, 0);
	goto_xpage(x); //Should set x address after setting y address
	for (i = 0; i < 8; i++) {
		goto_yaddress(temp_y, 0);
		write_data(font[(ch - 32)*8 + i]);
		temp_y += 1;
	}
}

void write_char_cont(int* x, int* y, unsigned char ch) {
	int i = 0;
	if (*y >= 127) { //End of line so go to next
		*y = 0;
		*x += 1;

	}
	goto_yaddress(*y, 0);
	goto_xpage(*x);
	for (i = 0; i < 8; i++) {
		goto_yaddress(*y, 0);
		write_data(font[(ch - 32)*8 + i]);
		*y += 1;
	}

}



void print_string(int* x, int* y, char* str) {
	while (*(str) != '\0') {
		write_char_cont(x, y, *(str));
		str++;
	}
}

void print_string2(int x, int y, char* str) {
	
	while (*(str) != '\0') {
		if(y>=127){
			y=0;
			x+=1;
		}
		write_char2(x, y, *(str));
		y+=8;
		str++;
	}
}

void print_big_string(int *x, int *y, char *str) {
	int i = 0, j=0;
	for (i = 0; i < bigfont[0]; i++) {
		goto_yaddress(*y, 0);
		for(j=0; j<5; j++){
			goto_xpage(*x+j);
			write_data(bigfont[5 * i + j+1]);
		}
		//        goto_xpage(*x);
		//        write_data(bigfont[2 * i + 1]);
		//        goto_xpage(*x + 1);
		//        write_data(bigfont[2 * i + 2]);
		*y += 1;
	}

	//    for (i = 0; i < bigfont[25]; i++) {
	//        goto_yaddress(*y, 0);
	//        goto_xpage(*x);
	//        write_data(bigfont[25 + 2 * i + 1]);
	//        goto_xpage(*x + 1);
	//        write_data(bigfont[25 + 2 * i + 2]);
	//        *y += 1;
	//    }

	//    goto_yaddress(*y, 0);
	//    goto_xpage(*x);
	//    write_data(font[3]);
	//    goto_xpage(*x + 1);
	//    write_data(font[4]);
	//
	//    goto_yaddress(*y, 0);
	//    goto_xpage(*x);
	//    write_data(font[5]);
	//    goto_xpage(*x + 1);
	//    write_data(font[6]);

}
void glcd_init() {
	//    //Set internal oscillator to 8MHz (default 4MHz)
	//    OSCCON &= 0b01111111;
	setbit(MCUCR, JTD); //Disable JTAG which is enabled by default for the m1284
	setbit(MCUCR, JTD); //Needs to be given twice according to datasheet (pg 275)
	
	DDRA = 0xFF;
	//DDRC = 0xFF;
	DDRD = 0xFF;
	
	PORTA = 0x00;
	//PORTC = 0x00;
	PORTD = 0x00;

	setbit(CS1PORT, CS1);
	setbit(CS2PORT, CS2);
	setbit(RSTPORT, RST);
	
	
	//Reset the display - after programming PIC, the screen gets offset by a few pixels at the top
	//This fixes it
	clearbit(RSTPORT, RST);
	wait(5);
	setbit(RSTPORT, RST);
	wait(5);

}

void do_raw_test(){
	wait(1000);
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	PORTA = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	setbit(CS1PORT, CS1); //Turn both halves on
	setbit(CS2PORT, CS2);
	setbit(RSTPORT, RST);
	clearbit(EPORT, E);
	
	//wait(100);
	//{
	//clearbit(RSTPORT, RST);
	//wait(100);
	//setbit(RSTPORT, RST);
	//wait(100);
	//}
	
	
	{
		clearbit(RSPORT, RS);
		clearbit(RWPORT, RW);
		DATA = 0b00111111;//Turn display on
		setbit(EPORT, E);
		wait(5);
		clearbit(EPORT, E);
		wait(5);
	}
	
	
	
	
	{
		clearbit(RSPORT, RS);
		clearbit(RWPORT, RW);
		DATA = 0b01000000;//Set Y address to 15
		setbit(EPORT, E);
		wait(10);
		clearbit(EPORT, E);
		wait(10);
	}
	
	{
		clearbit(RSPORT, RS);
		clearbit(RWPORT, RW);
		DATA = 0b10111100;//Set X page to 0
		setbit(EPORT, E);
		wait(10);
		clearbit(EPORT, E);
		wait(10);
	}
	
	{
		setbit(RSPORT, RS);
		clearbit(RWPORT, RW);
		DATA = 0b11111111;
		for(int i=0; i<10; i++){
			setbit(EPORT, E);
			wait(10);
			clearbit(EPORT, E);
			wait(10);
		}
	}
}

void show_message(int cursor_x, int cursor_y, char str[]){
	//print_string(&cursor_x,&cursor_y, str);
	print_string2(cursor_x,cursor_y, str);
}

void clear_message(int cursor_x, int cursor_y, char str[]){
	char blank[] = " ";
	int length = strlen(str);
	for(int i=0; i<length; i++){
		print_string(&cursor_y, &cursor_y, blank);
	}
}
