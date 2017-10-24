/*
 * We include the "contiki-net.h" file to get all the network functions.
 */
#include "contiki-net.h"
#include <imp_io.h>
#include <Peridot.h>

#include <mio.h>
#include <stdio.h>

// #define ethRxBuffer ((unsigned char*) 0xE000)
// #define ethTxBuffer ((unsigned char*) 0xE600)

static unsigned char ethRxBuffer[1530];
static unsigned char ethTxBuffer[1530];


PROCESS(ethernet_driver_process, "Ethernet driver");

unsigned short packet_size() {
  unsigned short size;
  size = ETH_RX_OCTETS_HI;  
  size = size << 8;
  size += ETH_RX_OCTETS_LO;
  return  size;
}

//void write_packet_size(unsigned short size) {
//  size += 1; //Adding custom header for priority (port is already included)
//  ETH_TX_OCTETS_LO = size & 0xFF;
//  ETH_TX_OCTETS_HI = size >> 8;
//}

//void enable_Rx() {
//  ETH_NIC_RX = ETH_NIC_RX | 0x02;
//}

//void disable_Rx() {
//  ETH_NIC_RX = ETH_NIC_RX & 0xFB;
//}
//
//void enable_Tx() {
//  ETH_NIC_TX = ETH_NIC_TX | 0x02;
//}
//
//void disable_Tx() {
//  ETH_NIC_TX = ETH_NIC_TX & 0xFB;
//}

//void set_done_Rx() {
//  ETH_NIC_RX = ETH_NIC_RX | 0x40;
//}

//void set_busy_Tx() {
//  ETH_NIC_TX = ETH_NIC_TX |Â 0x01;
//}

unsigned char is_busy_Tx() {
  return ETH_NIC_TX & 0x1;
}

void isrRx() __critical __interrupt {
  process_poll(&ethernet_driver_process);
  INT_ACK_2_DATA = 0x00;
  EI_2;
}

void isrTx() __critical __interrupt {
  is_busy_Tx();
  EI_2;
}

void port_to_bitmask(unsigned char* port) {
  if(*port != 0x1F) {
    *port = 1 << (0x1F & *port);
  }
}

// Pre: the packet to send is ethernet and starts in uip_buf[0] with an extra byte at the beginning 
// IN FORM OF BITMASK and has no CRC. uip_len has a proper value, including the port byte.
uint8_t send_packet_ethernet() {
  while(is_busy_Tx()); //The previous packet hasn't been sent so we need to wait for it
//  write_packet_size(uip_len);
  ETH_TX_OCTETS_LO = (uip_len+1) & 0xFF;
  ETH_TX_OCTETS_HI = (uip_len+1) >> 8;
  memcpy(ethTxBuffer+1, uip_buf, uip_len);
  ethTxBuffer[0] = 0x0D; //Priority
//  set_busy_Tx();
  ETH_NIC_TX = ETH_NIC_TX |Â 0x01;
  return 0;
}

uint8_t send_packet_ip() {
  uip_arp_out();
  port_to_bitmask(&uip_buf[0]);
  return send_packet_ethernet();
}

#define PACKET ((struct uip_eth_hdr *) &uip_buf[0])

int read_packet() {
  int i;
  unsigned int size = packet_size();
  if(size > 0) {
    uip_len = size - 1; //Removing priority byte
    // printf("Packet of size %d\n", uip_len);
    memcpy(uip_buf, ethRxBuffer+1, uip_len);
//    set_done_Rx();
    ETH_NIC_RX = ETH_NIC_RX | 0x40;
    INT_ACK_2_DATA = 0x40;
  } else {
    uip_len = 0;
  }
  return uip_len;
}

static void check_packet() {
  // write("Checking if we have received a packet");
  read_packet();
  if(uip_len > 0) {
    if(PACKET->type == UIP_HTONS(UIP_ETHTYPE_IP)) {
      uip_arp_ipin();
      uip_input();
      if(uip_len > 0) {
        send_packet_ip();
      }
    } else if(PACKET->type == UIP_HTONS(UIP_ETHTYPE_ARP)) {
      uip_arp_arpin();
      if(uip_len > 0) {
        port_to_bitmask(&uip_buf[0]);
        send_packet_ethernet();
      }
    }
  }
}

static void init_driver() {
  uip_arp_init();

  PortWrite(0x16,4, PortRead(0x16,4)|0x0803); // Enable z80 port; Define por header

  ETH_RX_ADDR_LO = (uintptr_t) ethRxBuffer & 0xFF;
  ETH_RX_ADDR_HI = (uintptr_t) ethRxBuffer >> 8;

  ETH_NIC_RX = 0x08;

  ETH_TX_ADDR_LO = (uintptr_t) ethTxBuffer & 0xFF;
  ETH_TX_ADDR_HI = (uintptr_t) ethTxBuffer >> 8;

  ETH_NIC_TX = 0x08;

//  enable_Tx();
  ETH_NIC_TX = ETH_NIC_TX | 0x02;
//  enable_Rx();
  ETH_NIC_RX = ETH_NIC_RX | 0x02;
}

PROCESS_THREAD(ethernet_driver_process, ev, data) {

  PROCESS_POLLHANDLER(check_packet());

  PROCESS_BEGIN();

  tcpip_set_outputfunc(send_packet_ip);

  init_driver();

  process_poll(&ethernet_driver_process);

  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_EXIT);

//  disable_Tx();
  ETH_NIC_TX = ETH_NIC_TX & 0xFB;
//  disable_Rx();
  ETH_NIC_RX = ETH_NIC_RX & 0xFB;

  PROCESS_END();
}
