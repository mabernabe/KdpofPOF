#ifndef __MIO__
#define __MIO__

enum {
	NEW_CHAR_EVENT,
	NEW_LINE_EVENT
};

void IO_init();
char read(unsigned char* s);
void write(unsigned char* s);

void IO_putchar(char c);
void ll_putchar(char c);

#endif
