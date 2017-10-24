#include "contiki.h"
#include "igmp.h"

#include <uip.h>
#include <ethernet_driver.h>
#include <mio.h>
#include <stdio.h>

#include <peridot.h>
#include <imp_io.h>
#include <imp_define.h>

#include "atu.h"

unsigned char M2H;

struct querier querier;
struct igmpService igmpService[IGMP_NUM_SERVICES];

void igmp_init() {
	unsigned short val;
	int i;
	M2H = 1; //Since only the port is inside uip_buf

	// Enable IGMP forwarding to CPU & Limit Egress Floods to unicast and known Multicast
	val = PortRead(0x10, 4);
	// write("0x10 0x4: 0x%04X\n", val);
	val = val & 0xFFF3;
	PortWrite(0x10, 4, val | 0x040C);
	// write("0x10 0x4: 0x%04X --> 0x%04X\n", val, PortRead(0x10, 4));

	val = PortRead(0x11, 4);
	// write("0x11 0x4: 0x%04X\n", val);
	val = val & 0xFFF3;
	PortWrite(0x11, 4, val | 0x040C);
	// write("0x11 0x4: 0x%04X\n", PortRead(0x11, 4));

	val = PortRead(0x12, 4);
	// write("0x12 0x4: 0x%04X\n", val);
	val = val & 0xFFF3;
	PortWrite(0x12,4, val | 0x040C);
	// write("0x12 0x4: 0x%04X\n", PortRead(0x12, 4));

	val = PortRead(0x13, 4);
	// write("0x13 0x4: 0x%04X\n", val);
	val = val & 0xFFF3;
	PortWrite(0x13,4, val | 0x040C);
	// write("0x13 0x4: 0x%04X\n", PortRead(0x13, 4));

	val = PortRead(0x14, 4);
	// write("0x14 0x4: 0x%04X\n", val);
	val = val & 0xFFF3;
	PortWrite(0x14, 4, val | 0x040C);
	// write("0x14 0x4: 0x%04X\n", PortRead(0x14, 4));

	val = PortRead(0x15, 4);
	// write("0x15 0x4: 0x%04X\n", val);
	val = val & 0xFFF3;
	PortWrite(0x15, 4, val | 0x040C);
	// write("0x15 0x4: 0x%04X\n", PortRead(0x15, 4));

	// Define Egress header and limit Multicast packets (No STP, .... )
	val = PortRead(0x16,4);
	PortWrite(0x15, 4, (val & 0xFFF3) | 0x0004);

	// Enable broadcast flood
	//G2Write(0x5, G2Read(0x5) | 0x1000);

	// Define IGMP CPU Port
	G1Write(0x1A, 0xB006);

	querier.valid = 0;
	for (i=0; i<IGMP_NUM_SERVICES; i++ ) {
		igmpService[i].valid=0;
	}
}

signed char igmpForward(unsigned char *buff, unsigned short len) {
	int payload;
	int type;
	unsigned char port;
	signed char forward = 0;
	signed char servicePort;
	signed char s;
	struct ip_addr grp;
	struct ip_addr client;

	if(len<30) return -1;

	payload = M2H+6+6+2+4*(buff[14+M2H]&0x0F);
	type = buff[payload];
	port = buff[0]&0x7;

	grp.b0 = buff[payload+4]; 
	grp.b1 = buff[payload+5]; 
	grp.b2 = buff[payload+6]; 
	grp.b3 = buff[payload+7]; 
	client.b0 = buff[M2H+26];
	client.b1 = buff[M2H+27];
	client.b2 = buff[M2H+28];
	client.b3 = buff[M2H+29];

	s = igmpFindService(&grp, &client);

//	s = igmpFindGroup(&grp);

	if(s>=0) servicePort = igmpService[s].to;
	else servicePort = -1;

	if(type == 0x11) {
		if(buff[M2H+30] == 224 && buff[M2H+31] == 0 && buff[M2H+32] == 0 && buff[M2H+33] == 1) {
		// General query to all ports but received port
//		    printf ("GenQ \n");
			forward = ~(1<<port);
		} else {
			// Group Query: If known, go to designated port. If Unknown to all but received port
			if(servicePort>=0) forward = 1<<servicePort;
			else forward = ~(1<<port);
//		    printf ("GrpQ \n");
		}
	}
	// Report to Querier
	if(type == 0x12 | type == 0x16) {
//		printf ("Report \n");
		if(querier.valid) {
			// Send to querier port if any. If querier is in the same port, do nothing.
			if((unsigned char) servicePort != querier.port) forward = 1<<querier.port;
			else forward = 0;
//			if((signed char) port != servicePort) { printf("Report coming from different port %d - %d\n", port, servicePort); }
		}
		// If no querier to all ports but received port
		else forward = ~(1<<port);
	}
	// Leave
	if(type == 0x17) {
//		printf ("Leave \n");
		if(querier.valid) {
			// Send to querier port if any. If querier is in the same port, do nothing.
			if(port != querier.port) forward = 1<<querier.port;
			else forward = 0;
		}
		// If no querier to all ports but received port
		else forward = ~(1<<port);
	} 
//	printf ("Fwd (G:0x%02X Q:0x%02X) 0x%02X to 0x%02X\n", servicePort, querier.port, port, forward);
	return forward;
}

unsigned char igmpPortVector(struct eth_addr *eth, int s) {
	int i;
	unsigned char port = 0;

	for (i=0; i<IGMP_NUM_SERVICES; i++ ) {
		if(igmpService[i].valid && i!=s) {
			if(igmpService[i].eth.b0 == eth->b0 &&
				igmpService[i].eth.b1 == eth->b1 &&
				igmpService[i].eth.b2 == eth->b2 &&
				igmpService[i].eth.b3 == eth->b3 &&
				igmpService[i].eth.b4 == eth->b4 &&
				igmpService[i].eth.b5 == eth->b5) 
			{
				
				port = port | (1<<igmpService[i].to);
			}
		}
	}
	return port;
}

signed char igmpHowMany(struct eth_addr *eth, signed char port) {
	int i;
	int cnt=0;

	for (i=0; i<IGMP_NUM_SERVICES; i++ ) {
		if(igmpService[i].valid) {
			if(igmpService[i].eth.b0 == eth->b0 &&
				igmpService[i].eth.b1 == eth->b1 &&
				igmpService[i].eth.b2 == eth->b2 &&
				igmpService[i].eth.b3 == eth->b3 &&
				igmpService[i].eth.b4 == eth->b4 &&
				igmpService[i].eth.b5 == eth->b5 &&
				(port>=0 && igmpService[i].to == (unsigned char) port || port<0)) 
			{
				cnt++;
			}
		}
	}
	return cnt;
}

signed char igmpProcess(unsigned char *buff, unsigned short len) {
	int payload;
	int type;
//	struct ip_addr src;
//	struct ip_addr dst;
//	struct ip_addr grp;
//
	if(len<30) return -1;

	payload = M2H+6+6+2+4*(buff[14+M2H]&0x0F);
	type = buff[payload];
	if(type == 0x11) {
		querier.src.b0 = buff[M2H+26];
		querier.src.b1 = buff[M2H+27];
		querier.src.b2 = buff[M2H+28];
		querier.src.b3 = buff[M2H+29];
		querier.eth.b0 = buff[M2H+0];
		querier.eth.b1 = buff[M2H+1];
		querier.eth.b2 = buff[M2H+2];
		querier.eth.b3 = buff[M2H+3];
		querier.eth.b4 = buff[M2H+4];
		querier.eth.b5 = buff[M2H+5];
		if(M2H) querier.port = buff[0]&0x7;
		if(querier.valid) {
			querier.period = IGMP_TIMEOUT- querier.timeout;
		} else {
			querier.period = IGMP_TIMEOUT;
		}
		querier.timeout = IGMP_TIMEOUT;
		querier.valid = 1;
		return 0;
	} else {
		if(type == 0x17) {
			signed char s;
			struct ip_addr grp;
			struct ip_addr client;

			grp.b0 = buff[payload+4]; 
			grp.b1 = buff[payload+5]; 
			grp.b2 = buff[payload+6]; 
			grp.b3 = buff[payload+7]; 
			client.b0 = buff[M2H+26];
			client.b1 = buff[M2H+27];
			client.b2 = buff[M2H+28];
			client.b3 = buff[M2H+29];

			s = igmpFindService(&grp, &client);
			
			if(s<0) {
				return -1;
			}
//			printf("-------Del Leave %d\n", s);
			atuIgmpDel(s);
			igmpService[s].valid=0;
			return 0;
		} else {
			if(type == 0x16) {
				int s;
				struct ip_addr grp;
				struct ip_addr client;

				grp.b0 = buff[payload+4]; 
				grp.b1 = buff[payload+5]; 
				grp.b2 = buff[payload+6]; 
				grp.b3 = buff[payload+7]; 
				client.b0 = buff[M2H+26];
				client.b1 = buff[M2H+27];
				client.b2 = buff[M2H+28];
				client.b3 = buff[M2H+29];

				s = igmpFindService(&grp, &client);

				if(s<0) {
					s= igmpGetServiceFree();
					if(s<0) {
//						printf("No free service\n");
						return -1;
					}
				} else {
					igmpService[s].timeout = IGMP_TIMEOUT;
					if(igmpService[s].to != buff[0]&0x7) {
//						printf(" Change port in IGMP service\n");
						igmpService[s].to = buff[0]&0x7;
						atuIgmpChange(s);
						return 0;
					}
				}


				igmpService[s].grp.b0 = buff[payload+4];
				igmpService[s].grp.b1 = buff[payload+5];
				igmpService[s].grp.b2 = buff[payload+6];
				igmpService[s].grp.b3 = buff[payload+7];
				igmpService[s].to = buff[0]&0x7;
				igmpService[s].eth.b0 = buff[M2H+0];
				igmpService[s].eth.b1 = buff[M2H+1];
				igmpService[s].eth.b2 = buff[M2H+2];
				igmpService[s].eth.b3 = buff[M2H+3];
				igmpService[s].eth.b4 = buff[M2H+4];
				igmpService[s].eth.b5 = buff[M2H+5];
				igmpService[s].client.b0 = buff[M2H+26];
				igmpService[s].client.b1 = buff[M2H+27];
				igmpService[s].client.b2 = buff[M2H+28];
				igmpService[s].client.b3 = buff[M2H+29];
				igmpService[s].timeout = IGMP_TIMEOUT;
				igmpService[s].valid = 1;
				atuIgmpAdd(s);
				return 0;
			} else return -1;
		}
	}
}

signed char igmpFindGroup(struct ip_addr *grp ) {
	int i;
	for (i=0; i<IGMP_NUM_SERVICES; i++) {
		if(igmpService[i].valid) {
			if(igmpService[i].grp.b0 == grp->b0 &&
				igmpService[i].grp.b1 == grp->b1 &&
				igmpService[i].grp.b2 == grp->b2 &&
				igmpService[i].grp.b3 == grp->b3 ) 
			{
				return i;
			}
		}
	}
	return -1;
}

signed char igmpFindService(struct ip_addr *grp, struct ip_addr *client) {
	int i;
	for (i=0; i<IGMP_NUM_SERVICES; i++) {
		if(igmpService[i].valid) {
			if(igmpService[i].grp.b0 == grp->b0 &&
				igmpService[i].grp.b1 == grp->b1 &&
				igmpService[i].grp.b2 == grp->b2 &&
				igmpService[i].grp.b3 == grp->b3 &&
				igmpService[i].client.b0 == client->b0 &&
				igmpService[i].client.b1 == client->b1 &&
				igmpService[i].client.b2 == client->b2 &&
				igmpService[i].client.b3 == client->b3 ) 
			{
				return i;
			}
		}
	}
	return -1;
}

void igmpTimer() {
	int i;
	for (i=0; i<IGMP_NUM_SERVICES; i++) {
		if(igmpService[i].valid && igmpService[i].timeout>1) {
			igmpService[i].timeout--;
		} else {
			if(igmpService[i].valid) {
//				printf("Del ATU Timer %d\n", i);
				atuIgmpDel(i);
			}
			igmpService[i].valid=0;
		}
	}
	if(querier.valid && querier.timeout>1) {
		querier.timeout--;
	} else {
		querier.valid=0;
	}
}

signed char igmpGetServiceFree() {
	int i;
	for (i=0; i<IGMP_NUM_SERVICES; i++) {
		if(igmpService[i].valid == 0) return i;
	}
	return -1;
}

void igmpServiceInfo(int service) {
	if(igmpService[service].valid) {
		printf("	Service %d: %d.%d.%d.%d %02x:%02x:%02x:%02x:%02x:%02x to port %d client %d.%d.%d.%d Time: %d\n", service,
			igmpService[service].grp.b0, igmpService[service].grp.b1, igmpService[service].grp.b2, igmpService[service].grp.b3, 
			igmpService[service].eth.b0, igmpService[service].eth.b1, igmpService[service].eth.b2, igmpService[service].eth.b3, igmpService[service].eth.b4, igmpService[service].eth.b5, 
			igmpService[service].to,
			igmpService[service].client.b0, igmpService[service].client.b1, igmpService[service].client.b2, igmpService[service].client.b3, 
			igmpService[service].timeout);
	}
}
void igmpInfo() {
	int i;
	if(querier.valid) {
		printf("	Querier: %d.%d.%d.%d at port %d Time: %d Period: %d\n", querier.src.b0, querier.src.b1, querier.src.b2, querier.src.b3, querier.port, querier.timeout, querier.period);
	}
	for (i=0; i<IGMP_NUM_SERVICES; i++) {
		igmpServiceInfo(i);
	}
}

PROCESS(igmp_process, "IGMP process\n");

PROCESS_THREAD(igmp_process, ev, data) {
	static struct etimer et;

	PROCESS_BEGIN();

	igmp_init();
	uip_listen_igmp();
	etimer_set(&et, CLOCK_SECOND);

	while(1) {
		PROCESS_WAIT_EVENT();
		if(ev == IGMP_PACKET_EVENT) {
			// write("IGMP packet arrived\n");
			if(igmpProcess(uip_buf, uip_len) >= 0) {
				signed char forward = igmpForward(uip_buf, uip_len);
				if(forward) {
					uip_buf[0] = forward & 0x1F;
					uip_len -= 4; //Remove CRC
					send_packet_ethernet();
				}
			}
		} else if(etimer_expired(&et)) {
			igmpTimer();
			atuTimer();
			etimer_reset(&et);
		} else if(ev == ATU_WRITE) {
//			write("=================");
			atuIgmpWrite();
//			write("=================");
		} else if(ev == ATU_DUMP) {
//			atuDump();
		} else if(ev == ATU_FLUSH) {
			atuFlush();
		} else if(ev == IGMP_INFO) {
			igmpInfo();
		}
	}

	PROCESS_END();
}
