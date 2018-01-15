#include <imp_io.h>
#include <imp_define.h>
#include <topaz.h>
#include <stdio.h>
#include "mio.h"
#include "clock.h"

#define SPACE 32
#define ESC 27

#define BUFF_LENGTH 1024
#define TIMER_IO_PERIOD 10

static volatile unsigned char buffer[BUFF_LENGTH];
static volatile int first;
static volatile int last;

unsigned char write_busy() {
	return COMM_STATUS & 0x10;
}

int ll_printf (const char *format, ...)
{
   va_list arg;
   int done;
   char aux[256];
   int i;

   va_start (arg, format);
   done = sprintf (aux, format, arg);
   va_end (arg);
   for (i=0; i<256 && aux[i]; i++) ll_putchar(aux[i]);

   return done;
}

void ll_putchar(char c) {
	while (0 != (COMM_STATUS & 0x10));
	COMM_WR_DATA = c;
}

void isrT1() __critical __interrupt {
	unsigned char ctl;
	ctl = TIMER1_CTL;
	if(!write_busy()) {
		if(first != last) {
			COMM_WR_DATA = buffer[first++];
			first %= BUFF_LENGTH;
		}
	}
	EI_2;
}


void IO_init() {
	first = 0;
	last = 0;

	TIMER1_VAL_LO = TIMER_IO_PERIOD & 0xFF;;
	TIMER1_VAL_HI = TIMER_IO_PERIOD / 256;
	TIMER1_CTL = 0x6;
}

void IO_putchar(char c) {
	if(c == '\n') {
		putchar('\r'); //This is for printing in a windows computer
	}
	DI;
	buffer[last++] = c;
	last %= BUFF_LENGTH;
	EI_2;
}

/*
extern char data_logs_html[];

void html_putchar(char c) {
	static int offset = -1;
	static int i = 0;

	if(c == '\n') {
		printf("<br>");
	}

	if(offset == -1) {
		while(data_logs_html[++offset] != '#');
	}

	if(data_logs_html[offset + i + 1] == '@') { 
		memcpy(&data_logs_html[offset], &data_logs_html[offset+1], i);
	} else {
		++i;
	}
	data_logs_html[offset + i] = c;
}
*/

#include "telnetd.h"

void telnet_putchar(char c) {
	process_post_synch(&telnetd_process, NEW_CHAR_EVENT, &c);
	if(c == '\n') {
		process_post(&telnetd_process, NEW_LINE_EVENT, NULL);
	}
}

void putchar(char c){
#if defined(IO_CONSOLE)
	IO_putchar(c);
//#elif defined(IO_HTML)
//	html_putchar(c);
#elif defined(IO_TELNET)
	telnet_putchar(c);
#else
	if (connected) {
		telnet_putchar(c);
	}	
	IO_putchar(c);
#endif
}


char getchar(void) {
	char c = 0;
	while(0 == (COMM_STATUS & 0x01));
	c = COMM_RD_DATA;
	return c;
}

char read(unsigned char* s) {
	unsigned int i;
	unsigned char c;
	for(i = 0; i < BUFF_LENGTH-1; ++i) {
		c = getchar();
		putchar(c);
		s[i] = c;
		if(c < 32) {
			printf("\r\n");
			s[i+1] = 0;
			return 1;
		}
	}
	s[i+1] = 0;
	return 0;
}

void write(unsigned char* s) {
	// printf("%ldms: ", clock_time());
	printf(s);
	printf("\r\n");
}

void uip_log(char* s) {
	write(s);
}
