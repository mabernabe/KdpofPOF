#ifndef __ETHERNET_DRIVER__
#define __ETHERNET_DRIVER__

#include "contiki.h"

PROCESS_NAME(ethernet_driver_process);

uint8_t send_packet_ip();
uint8_t send_packet_ethernet();

#define ETH_INFO 2

#endif
