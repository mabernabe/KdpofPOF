#include "contiki.h"

#include "mio.h"
#include "topaz.h"
/* devices */
#include "contiki-net.h"
#include "uip.h"
#include "dhcp_process.h"
#include "ethernet_driver.h"
//#include "webserver-nogui.h"
//#include "is-alive.h"
#include "igmp.h"
#include "telnetd.h"
#include "contiki-global.h"

#include <mio.h>
#include <stdio.h>

uip_lladdr_t eth_addr = { {0xff,0xff,0xff,0xff,0xff,0xff} };
char serial_number[7]= "012345";
unsigned char fw_version[2]={'2','0'};
//uip_ipaddr_t host_addr = { { 192, 168, 1, 250 } };
//const uip_ipaddr_t def_route_addr = { { 192, 168, 1, 1 } };
//const uip_ipaddr_t net_mask = { { 255, 255, 255, 0 } };
const uip_lladdr_t default_eth_addr = { {0, 0x50, 0x43, 0x99, 0x99, 0x99} };

/*---------------------------------------------------------------------------*/

void nmi_isr() __critical __interrupt {
}

void initInterruptions() {
  INT_ACK_2_DATA = 0x40; // Ethernet driver
  INT_ACK_3_DATA = 0x48; // Ethernet driver
  INT_ACK_0_DATA = 0x50; // Contiki internal timer
  INT_ACK_1_DATA = 0x52; // Timer for IO console through SMI
  __asm
    push af
    ld a,#0x0
    ld i, a
    pop af
  __endasm;
  EI_2;
}

int main(void) {

  int i;

  ll_putchar('\r');
  ll_putchar('\n');
  // First step boot
  ll_putchar('B');
  ll_putchar('O');
  ll_putchar('O');
  ll_putchar('T');
  ll_putchar(':');
  ll_putchar(' ');
  ll_putchar('0');

  // Resetting the switch
  EEPROM_SPEED = EEPROM_SPEED | 0x80;
  EEPROM_SPEED = EEPROM_SPEED & 0x7F;

  // Second step boot
  ll_putchar('1');

  clock_init();
  ll_putchar('2');
  IO_init(); // Only necessary when using SMI console
  ll_putchar('3');
  initInterruptions();
  ll_putchar('4');

  /* initialize process manager. */
  process_init();
  ll_putchar('5');

  uip_init();
  ll_putchar('6');

  // Get ETH Addr from EEPROM
  //eeprom2ram(eth_addr.addr, (void *) 0xB, sizeof(eth_addr));
  if (eth_addr.addr[0]==0xFF) memcpy(eth_addr.addr, default_eth_addr.addr, sizeof(eth_addr));
  ll_putchar('7');

  IO_putchar('\n');
  IO_putchar('E');
  IO_putchar('A');
  IO_putchar(' ');
  for (i=0; i<sizeof(eth_addr); i++) {
	IO_putchar('0'+(eth_addr.addr[i]>>4));
	IO_putchar('0'+(eth_addr.addr[i]&0xF));
  	if (i<(sizeof(eth_addr)-1)) IO_putchar(':');
  }
  IO_putchar('\r');
  IO_putchar('\n');

  // Get Serial Number from EEPROM
  //eeprom2ram(serial_number, (void *) 0x11, sizeof(serial_number));
  IO_putchar('S');
  IO_putchar('N');
  IO_putchar(' ');
  for (i=0; i<sizeof(serial_number); i++) {
     if (serial_number[i]) IO_putchar(serial_number[i]);
     else break;
  }
  IO_putchar('\r');
  IO_putchar('\n');


  ll_putchar('8');
  write("\n------------------");
  write("Initializing contiki");

  uip_setethaddr(eth_addr);
  // uip_sethostaddr(&host_addr); // Only necessary when not using dhcp
  // uip_setdraddr(&def_route_addr);
  // uip_setnetmask(&net_mask);

  ll_putchar('9');
  /* start services */
  process_start(&etimer_process, NULL);
  ll_putchar('A');
  //process_start(&is_alive_process, NULL); // For debugging
  ll_putchar('B');

  process_start(&tcpip_process, NULL);
  ll_putchar('C');
  process_start(&dhcp_process, NULL);
  ll_putchar('D');
  process_start(&igmp_process, NULL);
  ll_putchar('E');
//  process_start(&webserver_nogui_process, NULL);
  ll_putchar('F');
  process_start(&telnetd_process, NULL);
  ll_putchar('G');
  process_start(&ethernet_driver_process, NULL); // This order is important
  ll_putchar('H');
  ll_putchar('\r');
  ll_putchar('\n');

  write("Services started");
  while(1) {
    process_run();
  }
}
