/*
 * glcdcontrols.h
 *
 * Created: 11/10/2013 1:11:39 PM
 *  Author: ryder
 */ 


#ifndef GLCDCONTROLS_H_
#define GLCDCONTROLS_H_

extern int is_inverted;
void executeCommand();
void glcd_on();
void goto_yaddress(int yadd, int both_halves);
void goto_xpage(int xadd);
void gotoxy(int x, int y);
void write_data(short data);

void draw_picture(const char* pic);
void glcd_clear();
void write_char(int *x, int *y, unsigned char ch);
void write_char_cont(int* x, int* y, unsigned char ch);
void print_string(int* x, int* y, char* str);
void print_big_string(int *x, int *y, char *str);
void glcd_init();
void do_raw_test();
void show_message(int cursor_x, int cursor_y, char str[]);
void clear_message(int cursor_x, int cursor_y, char str[]);

#endif /* GLCDCONTROLS_H_ */