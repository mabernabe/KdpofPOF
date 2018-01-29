#ifndef __IGMP__
#define __IGMP__

#include "contiki.h"

enum {
	IGMP_PACKET_EVENT,
	ATU_WRITE,
	ATU_DUMP,
	ATU_FLUSH,
	IGMP_INFO
};

#define IGMP_NUM_SERVICES 20
#define IGMP_TIMEOUT 150


struct ip_addr { unsigned char b0; unsigned char b1; unsigned char b2; unsigned char b3;};
struct eth_addr { unsigned char b0; unsigned char b1; unsigned char b2; unsigned char b3; unsigned char b4; unsigned char b5;};

//extern struct querier { unsigned char port; unsigned char valid; } querier;
extern struct querier { struct ip_addr src; struct eth_addr eth; unsigned char port; unsigned char valid; unsigned short timeout; unsigned short period;} querier;
extern struct igmpService { struct ip_addr grp; struct eth_addr eth; struct ip_addr client; unsigned char from; unsigned char to; unsigned char valid; unsigned short timeout;} igmpService[IGMP_NUM_SERVICES];
extern unsigned char M2H;

PROCESS_NAME(igmp_process);


void igmpInit();
unsigned int igmpIsIgmp(unsigned char *buff, unsigned short len);
void igmpPrint(unsigned char *buff, unsigned short len);
signed char igmpProcess(unsigned char *buff, unsigned short len);
signed char igmpForward(unsigned char *buff, unsigned short len);
void igmpInfo();
void igmpServiceInfo(int service);
void igmpTimer();
signed char igmpGetServiceFree();
signed char igmpFindService(struct ip_addr *grp, struct ip_addr *client);
signed char igmpFindGroup(struct ip_addr *grp);
unsigned char igmpPortVector(struct eth_addr *eth, int s);
signed char igmpHowMany(struct eth_addr *eth, signed char port);
signed char igmpNumServices(void);


#endif
