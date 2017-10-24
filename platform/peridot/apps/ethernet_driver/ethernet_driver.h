#ifndef __ETHERNET_DRIVER__
#define __ETHERNET_DRIVER__

#include "contiki.h"

PROCESS_NAME(ethernet_driver_process);

void send_packet_ip();
void send_packet_ethernet();

#endif
