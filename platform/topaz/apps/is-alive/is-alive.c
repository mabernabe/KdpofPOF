#include "contiki.h"

#include <mio.h>
#include <stdio.h>
#include "etimer.h"
#include "clock.h"

/*---------------------------------------------------------------------------*/
PROCESS(is_alive_process, "Is alive process");

void ll_putchar(char);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(is_alive_process, ev, data) {
	static struct etimer et;
	PROCESS_BEGIN();

	ll_putchar('a'); ll_putchar('0');
	etimer_set(&et, 10*CLOCK_SECOND);
	ll_putchar('a'); ll_putchar('1');

	while(1) {
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		printf("At %ld still alive\n", clock_time());
		etimer_reset(&et);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
