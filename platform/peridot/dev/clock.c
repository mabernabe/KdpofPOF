#include <clock.h>
#include <contiki-conf.h>
#include <imp_io.h>
#include <stdio.h>
#include "etimer.h"

#define TIMER_INTER_PERIOD 10

static clock_time_t millis;

void isrT0() __critical __interrupt {
	unsigned char ctl;
	ctl = TIMER0_CTL;
	++millis;
	etimer_request_poll();
	EI_2;
}

void clock_init() {
	millis = 0;

	TIMER0_VAL_LO = TIMER_INTER_PERIOD & 0xFF;;
	TIMER0_VAL_HI = TIMER_INTER_PERIOD / 256;
	TIMER0_CTL = 0x6;
}

clock_time_t clock_time() {
	return millis;
}

unsigned long clock_seconds(void) {
	return millis / CLOCK_CONF_SECOND;
}