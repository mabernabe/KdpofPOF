#include "contiki.h"

#include <mio.h>
#include <stdio.h>
#include <topaz.h>
#include "igmp.h"
#include "ethernet_driver.h"
#include "contiki-global.h"

void print_help() {
	write("help:		Shows this help");
	write("reset:		Resets the switch (use only with eeprom)");
	write("info:		Dump General information");
	write("eth:		Dump Ethernet stats");
	write("igmp:            Dump Igmp services");
	write("pof:		Dump POF information");
	write("atu_flush:	Flushes ATU Tables");
	write("atu_dump:	Dump ATU tables");
	write("time:		Shows the time in miliseconds since startup");
}

void pof_info() {

	signed short lm;
	signed short pwr;
	unsigned short reg;

	printf("POF Information\r\n");
	printf("===============\r\n");
	printf("Port 0x10\r\n");
	printf("    Link:        %d\r\n", (ExtC22Read(0x10, 1)&0x4)==0x4);

	lm = ExtC22Read(0x10, 19)&0x0FFF;
	if ((lm&0x800)==0x800) lm = lm | 0xF000; // Sign extension
	lm=lm/3;
	printf("    Link Margin: %d.%d dB\r\n", lm/256, lm>=0 ? ((lm&0xFF)*100)/256 : ((-lm)&0xFF)*100)/256;

	/*
	pwr = ExtC22Read(0x10, 23)&0x3FFF;
	printf("    Optical Pwr: %d\r\n", pwr);
	if ((pwr&0x2000)==0x2000) pwr = pwr | 0xC000; // Sign extension
	printf("    Optical Pwr: %d\r\n", pwr);
	*/

	ExtC22Write(0x10,13,0x001E);
	ExtC22Write(0x10,14,0x0214);
	ExtC22Write(0x10,13,0x401E);
	reg = ExtC22Read(0x10, 14);
	printf("    Sync       : 0x%x\r\n", reg);

	printf("Port 0x15\r\n");
	printf("    Link:        %d\r\n", (ExtC22Read(0x15, 1)&0x4)==0x4);

	lm = ExtC22Read(0x15, 19)&0x0FFF;
	if ((lm&0x800)==0x800) lm = lm | 0xF000; // Sign extension
	lm=lm/3;
	printf("    Link Margin: %d.%d dB\r\n", lm/256, lm>=0 ? ((lm&0xFF)*100)/256 : ((-lm)&0xFF)*100)/256;

	/*
	pwr = ExtC22Read(0x15, 23)&0x3FFF;
	printf("    Optical Pwr: %d\r\n", pwr);
	if ((pwr&0x2000)==0x2000) pwr = pwr | 0xC000; // Sign extension
	printf("    Optical Pwr: %d\r\n", pwr);
	*/

	ExtC22Write(0x15,13,0x001E);
	ExtC22Write(0x15,14,0x0214);
	ExtC22Write(0x15,13,0x401E);
	reg = ExtC22Read(0x15, 14);
	printf("    Sync       : 0x%x\r\n", reg);


}

PROCESS(shell_process, "Shell process");

PROCESS_THREAD(shell_process, ev, data) {
	int i;
	char* line;
	char* command;
	char* args;
	PROCESS_BEGIN();
 
 	write("Starting shell - Enter 'help' for a list of commands");

 	while(1) {
 		PROCESS_WAIT_EVENT_UNTIL(ev == NEW_LINE_EVENT);
 		line = (char*) data;
 		i = 0;
 		command = line;
 		args = NULL;
 		while(line[i] != '\0' && line[i] != '\n') {
 			if(line[i] == ' ') {
 				line[i] = '\0'; // split of command | args
 				args = &line[i+1];
 				break;
 			}
 			++i;
 		}
 		if(strcmp(command, "help") == 0) {
 			print_help();
 		} else if(strcmp(command, "reset") == 0){
 			ResetSwitch();
 		} else if(strcmp(command, "info") == 0){
 			write("General information:");
 			write("--------------------");
 			printf("SN: %s\n", serial_number);
 			printf("FW: %c.%c\n", fw_version[0], fw_version[1]);
			printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", uip_lladdr.addr[0],uip_lladdr.addr[1],
			                                               uip_lladdr.addr[2],uip_lladdr.addr[3],
			                                               uip_lladdr.addr[4],uip_lladdr.addr[5] );
			printf("IP: %d.%d.%d.%d\n", uip_hostaddr.u8[0],uip_hostaddr.u8[1],
			                            uip_hostaddr.u8[2],uip_hostaddr.u8[3] );
			printf("Route: %d.%d.%d.%d\n", uip_draddr.u8[0],uip_draddr.u8[1],
			                               uip_draddr.u8[2],uip_draddr.u8[3] );
			printf("Netmask: %d.%d.%d.%d\n", uip_netmask.u8[0],uip_netmask.u8[1],
			                                 uip_netmask.u8[2],uip_netmask.u8[3] );

 		} else if(strcmp(command, "atu_flush") == 0){
 			process_post(&igmp_process, ATU_FLUSH, NULL);
 		} else if(strcmp(command, "atu_dump") == 0){
 			process_post(&igmp_process, ATU_DUMP, NULL);
 		} else if(strcmp(command, "atu_write") == 0){
 			process_post(&igmp_process, ATU_WRITE, NULL);
 		} else if(strcmp(command, "igmp") == 0){
 			process_post(&igmp_process, IGMP_INFO, NULL);
 		} else if(strcmp(command, "eth") == 0){
 			process_post(&ethernet_driver_process, ETH_INFO, NULL);
 		} else if(strcmp(command, "time") == 0){
 			printf("%ld ms\n", clock_time());
 		} else if(strcmp(command, "pof") == 0){
			pof_info();
 		} else {
 			write("Unrecognized command. Type help to see the available ones");
 		}
		printf(":> ");
 	}

	PROCESS_END();
}
