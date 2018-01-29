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
  printf("Time: %ld ms\n", clock_time());
  printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", uip_lladdr.addr[0],uip_lladdr.addr[1],
                                                 uip_lladdr.addr[2],uip_lladdr.addr[3],
                                                 uip_lladdr.addr[4],uip_lladdr.addr[5] );
  printf("IP: %d.%d.%d.%d\n", uip_hostaddr.u8[0],uip_hostaddr.u8[1],
                              uip_hostaddr.u8[2],uip_hostaddr.u8[3] );
  printf("Route: %d.%d.%d.%d\n", uip_draddr.u8[0],uip_draddr.u8[1],
                                 uip_draddr.u8[2],uip_draddr.u8[3] );
  printf("Netmask: %d.%d.%d.%d\n", uip_netmask.u8[0],uip_netmask.u8[1],
                                   uip_netmask.u8[2],uip_netmask.u8[3] );
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
