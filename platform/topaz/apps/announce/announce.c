#include "contiki.h"

#include "mio.h"
#include <topaz.h>
#include "contiki-net.h"
#include "etimer.h"
#include "clock.h"
#include "announce.h"
#include "uip.h"
#include "simple-udp.h"
#include "contiki-global.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(announce_process, "Announce process");

#define UDP_PORT 9875

#define SEND_INTERVAL		(5 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))

static struct simple_udp_connection broadcast_connection;

/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
//  printf("Data received on port %d from port %d with length %d\n", receiver_port, sender_port, datalen);
}

#define SAP_V         0
#define SAP_V_VER     0x20
#define SAP_V_A       0x00
#define SAP_V_R       0x00
#define SAP_V_T       0x00
#define SAP_V_E       0x00
#define SAP_V_C       0x00
#define SAP_AUTH_LEN  1
#define SAP_ID_HASH_H 2
#define SAP_ID_HASH_L 3
#define SAP_SOURCE    4
#define SAP_TYPE      8
#define SAP_DATA      16

static int pof_info(char *buff) {

	signed short lm;
	unsigned short reg;
	char aux[80];

	buff[0]=0;

	sprintf(aux,"POF Information\r\n");
	strcat(buff, aux);
	sprintf(aux,"===============\r\n");
	strcat(buff, aux);
	sprintf(aux,"Port 0x10\r\n");
	strcat(buff, aux);
	sprintf(aux,"    Link:        %d\r\n", (ExtC22Read(0x10, 1)&0x4)==0x4);
	strcat(buff, aux);

	lm = ExtC22Read(0x10, 19)&0x0FFF;
	if ((lm&0x800)==0x800) lm = lm | 0xF000; // Sign extension
	lm=lm/3;
	sprintf(aux, "    Link Margin: %d.%d dB\r\n", lm/256, lm>=0 ? ((lm&0xFF)*100)/256 : ((-lm)&0xFF)*100)/256;
	strcat(buff, aux);

	ExtC22Write(0x10,13,0x001E);
	ExtC22Write(0x10,14,0x0214);
	ExtC22Write(0x10,13,0x401E);
	reg = ExtC22Read(0x10, 14);
	sprintf(aux, "    Sync       : 0x%x\r\n", reg);
	strcat(buff, aux);

	sprintf(aux, "Port 0x15\r\n");
	strcat(buff, aux);
	sprintf(aux, "    Link:        %d\r\n", (ExtC22Read(0x15, 1)&0x4)==0x4);
	strcat(buff, aux);

	lm = ExtC22Read(0x15, 19)&0x0FFF;
	if ((lm&0x800)==0x800) lm = lm | 0xF000; // Sign extension
	lm=lm/3;
	sprintf(aux, "    Link Margin: %d.%d dB\r\n", lm/256, lm>=0 ? ((lm&0xFF)*100)/256 : ((-lm)&0xFF)*100)/256;
	strcat(buff, aux);

	ExtC22Write(0x15,13,0x001E);
	ExtC22Write(0x15,14,0x0214);
	ExtC22Write(0x15,13,0x401E);
	reg = ExtC22Read(0x15, 14);
	sprintf(aux, "    Sync       : 0x%x\r\n", reg);
	strcat(buff, aux);

	return strlen(buff);
}



/*---------------------------------------------------------------------------*/
PROCESS_THREAD(announce_process, ev, data) {
        static struct etimer periodic_timer;
        static struct etimer send_timer;
//	uip_ipaddr_t addr = { { 224, 2, 127, 254 } };
	static uip_ipaddr_t addr;
	static uint8_t message[48];
	int len_type;
	int len_message;
	uip_ipaddr_t *p_addr;


	PROCESS_BEGIN();

//	01:00:5e:02:7f:fe
//	224.2.127.254
//
        uip_ipaddr(&addr, 224,2,127,254);
        // SAP Message
	message[SAP_V] = SAP_V_VER | SAP_V_A | SAP_V_R | SAP_V_T | SAP_V_E;
	message[SAP_AUTH_LEN] = 0;
        message[SAP_ID_HASH_H] = 0x01; // Put part of the serial number as hash ID
        message[SAP_ID_HASH_L] = 0x23;
	p_addr = (uip_ipaddr_t *) (&message[SAP_SOURCE]);
        uip_ipaddr_copy(p_addr, &uip_hostaddr);
        sprintf (&message[SAP_TYPE], "text/plain");
//        message[SAP_DATA]
        simple_udp_register(&broadcast_connection, UDP_PORT,
                      &addr, UDP_PORT,
                      receiver);

        etimer_set(&periodic_timer, SEND_INTERVAL);

	while(1) {
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
                etimer_reset(&periodic_timer);
                etimer_set(&send_timer, SEND_TIME);

                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
	        p_addr = (uip_ipaddr_t *) (&message[SAP_SOURCE]);
                uip_ipaddr_copy(p_addr, &uip_hostaddr);
		len_type = strlen(&message[SAP_TYPE]);
		len_message = pof_info( &message[SAP_TYPE + len_type +1]);

                printf("Sending broadcast to ipaddr=%d.%d.%d.%d\n", uip_ipaddr_to_quad(&addr));
                simple_udp_sendto(&broadcast_connection, message, len_message + len_type + SAP_TYPE, &addr);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
