#include "contiki-net.h"
#include "dhcpc.h"
#include "mio.h"
#include <stdio.h>

extern const uip_lladdr_t eth_addr;

void dhcpc_configured(const struct dhcpc_state *s) {
  uip_sethostaddr(&s->ipaddr);
  uip_setnetmask(&s->netmask);
  uip_setdraddr(&s->default_router);
  write("IP assigned");
}

void dhcpc_unconfigured(const struct dhcpc_state *s) {
  uip_log("DHCP unconfigured");
}

PROCESS(dhcp_process, "DHCP");

PROCESS_THREAD(dhcp_process, ev, data) {
  PROCESS_BEGIN();
  
  write("Starting DHCP");

  dhcpc_init(&eth_addr, sizeof(eth_addr));
  dhcpc_request();

  while(1) {
    PROCESS_WAIT_EVENT();
    dhcpc_appcall(ev, data);
  }

  PROCESS_END();
}
