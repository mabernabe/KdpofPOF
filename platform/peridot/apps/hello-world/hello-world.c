#include "contiki.h"

#include <mio.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data) {
	PROCESS_BEGIN();
  
	while(1) {
		PROCESS_WAIT_EVENT();
		printf("Hello-world event: %d\n", ev);
		printf("-----Dir a = %p\n", &a);
		printf("-----Dir b = %p\n", &b);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
