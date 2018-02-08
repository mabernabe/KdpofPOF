#include "atu.h"
#include "igmp.h"

#include <stdio.h>
#include <mio.h>
#include <string.h>
#include <TopazTypes.h>
#include <Topaz.h>

/*
static void atuPrint(ATU_ENTRY *atuEntry) {
	printf("--------- ATU_DATA ---------\n");
	printf("ATUOp            = %d\n", atuEntry->ATUOp);
	printf("MAC Address  = %02x-%02x-%02x-%02x-%02x-%02x\n",
		atuEntry->ATUByte0, atuEntry->ATUByte1, atuEntry->ATUByte2, atuEntry->ATUByte3, atuEntry->ATUByte4, atuEntry->ATUByte5);

	// printf("MAC_FPri         = %d\n", atuEntry->MAC_FPri);
	// printf("ATUFullViolation = %d\n", atuEntry->ATUFullViolation);
	// printf("MissViolation    = %d\n", atuEntry->MissViolation);
	// printf("MemberViolation  = %d\n", atuEntry->MemberViolation);
	// printf("AgeOutViolation  = %d\n", atuEntry->AgeOutViolation);
	// printf("MAC_QPri         = %d\n", atuEntry->MAC_QPri);
	// printf("MAC_QPri         = %d\n", atuEntry->MAC_QPri);
	// printf("EntryState_SPID  = %d\n", atuEntry->EntryState_SPID);
	// printf("LAG              = %d\n", atuEntry->LAG);

	// printf("FID              = %d\n", atuEntry->FID);
	printf("PortVec          = %x\n", atuEntry->PortVec);
}
*/

static int atuFind(ATU_ENTRY *atuEntry, struct eth_addr *eth) {

	memset(&atuEntry, 0, sizeof(ATU_ENTRY));

	// Go to the begining
	while(atuEntry->ATUByte0 != 0xFF &&
		atuEntry->ATUByte1 != 0xFF &&
		atuEntry->ATUByte2 != 0xFF &&
		atuEntry->ATUByte3 != 0xFF &&
		atuEntry->ATUByte4 != 0xFF &&
		atuEntry->ATUByte5 != 0xFF) 
	{
		atuGetEntryNext(atuEntry);
	}

	memset(&atuEntry, 0, sizeof(ATU_ENTRY));

	while(atuEntry->ATUByte0 != 0xFF &&
		atuEntry->ATUByte1 != 0xFF &&
		atuEntry->ATUByte2 != 0xFF &&
		atuEntry->ATUByte3 != 0xFF &&
		atuEntry->ATUByte4 != 0xFF &&
		atuEntry->ATUByte5 != 0xFF) 
	{
		atuGetEntryNext(atuEntry);
		if(atuEntry->ATUByte0 == eth->b0 &&
			atuEntry->ATUByte1 == eth->b1 &&
			atuEntry->ATUByte2 == eth->b2 &&
			atuEntry->ATUByte3 == eth->b3 &&
			atuEntry->ATUByte4 == eth->b4 &&
			atuEntry->ATUByte5 == eth->b5) 
		{
			return 1;
		}
	}

	memset(&atuEntry, 0, sizeof(ATU_ENTRY));

	while(atuEntry->ATUByte0 != 0xFF &&
		atuEntry->ATUByte1 != 0xFF &&
		atuEntry->ATUByte2 != 0xFF &&
		atuEntry->ATUByte3 != 0xFF &&
		atuEntry->ATUByte4 != 0xFF &&
		atuEntry->ATUByte5 != 0xFF) 
	{
		atuGetEntryNext(atuEntry);
		if(atuEntry->ATUByte0 == eth->b0 &&
			atuEntry->ATUByte1 == eth->b1 &&
			atuEntry->ATUByte2 == eth->b2 &&
			atuEntry->ATUByte3 == eth->b3 &&
			atuEntry->ATUByte4 == eth->b4 &&
			atuEntry->ATUByte5 == eth->b5) 
		{
			return 1;
		}
	}
	return 0;
}


/*
void atuDump() {
	ATU_ENTRY atuEntry;

	memset(&atuEntry, 0, sizeof(ATU_ENTRY));

	// Go to the begining
	while(atuEntry.ATUByte0 != 0xFF &&
		atuEntry.ATUByte1 != 0xFF &&
		atuEntry.ATUByte2 != 0xFF &&
		atuEntry.ATUByte3 != 0xFF &&
		atuEntry.ATUByte4 != 0xFF &&
		atuEntry.ATUByte5 != 0xFF) 
	{
		atuGetEntryNext(&atuEntry);
	}

	memset(&atuEntry, 0, sizeof(ATU_ENTRY));

	while(atuEntry.ATUByte0 != 0xFF &&
		atuEntry.ATUByte1 != 0xFF &&
		atuEntry.ATUByte2 != 0xFF &&
		atuEntry.ATUByte3 != 0xFF &&
		atuEntry.ATUByte4 != 0xFF &&
		atuEntry.ATUByte5 != 0xFF) 
	{
		atuGetEntryNext(&atuEntry);
		if(atuEntry.ATUByte0 == 0x01 &
		   atuEntry.ATUByte0 == 0x00 &
		   atuEntry.ATUByte0 == 0x5e ) {
			atuPrint(&atuEntry);
		}
	}
}
*/

void atuIgmpAdd(int s) {
	ATU_ENTRY atuEntry;
	if(igmpService[s].valid) {
		unsigned char portVector;
		unsigned char aux;

		portVector = igmpPortVector(&igmpService[s].eth, -1);

		memset(&atuEntry, 0, sizeof(ATU_ENTRY));
		atuEntry.ATUByte0 = igmpService[s].eth.b0;
		atuEntry.ATUByte1 = igmpService[s].eth.b1; //39-32
		atuEntry.ATUByte2 = igmpService[s].eth.b2;
		atuEntry.ATUByte3 = igmpService[s].eth.b3;
		atuEntry.ATUByte4 = igmpService[s].eth.b4; 
		atuEntry.ATUByte5 = igmpService[s].eth.b5;
/*
		aux = igmpService[s].eth.b0;
		atuEntry.ATUByte0 = aux;
		aux = igmpService[s].eth.b1; //39-32
		atuEntry.ATUByte1 = aux;
		aux = igmpService[s].eth.b2;
		atuEntry.ATUByte2 = aux;
		aux = igmpService[s].eth.b3;
		atuEntry.ATUByte3 = aux;
		aux = igmpService[s].eth.b4; 
		atuEntry.ATUByte4 = aux;
		aux = igmpService[s].eth.b5;
		atuEntry.ATUByte5 = aux;
*/
		atuEntry.EntryState_SPID = 7;
		atuEntry.PortVec = portVector;
		atuEntry.FID = 0;
		atuAddEntry(&atuEntry);
//		printf("Added entry for service %d\n", s);
//		atuPrint(&atuEntry);
//		igmpServiceInfo(s);
	}
}

void atuIgmpDel(int s) {
	ATU_ENTRY atuEntry;
	if(igmpService[s].valid) {
		unsigned char portVector;
		unsigned char aux;
		portVector = igmpPortVector(&igmpService[s].eth, s);

		memset(&atuEntry, 0, sizeof(ATU_ENTRY));
		atuEntry.ATUByte0 = igmpService[s].eth.b0;
		atuEntry.ATUByte1 = igmpService[s].eth.b1; //39-32
		atuEntry.ATUByte2 = igmpService[s].eth.b2;
		atuEntry.ATUByte3 = igmpService[s].eth.b3;
		atuEntry.ATUByte4 = igmpService[s].eth.b4; 
		atuEntry.ATUByte5 = igmpService[s].eth.b5;
/*
		aux = igmpService[s].eth.b0;
		atuEntry.ATUByte0 = aux;
		aux = igmpService[s].eth.b1; //39-32
		atuEntry.ATUByte1 = aux;
		aux = igmpService[s].eth.b2;
		atuEntry.ATUByte2 = aux;
		aux = igmpService[s].eth.b3;
		atuEntry.ATUByte3 = aux;
		aux = igmpService[s].eth.b4; 
		atuEntry.ATUByte4 = aux;
		aux = igmpService[s].eth.b5;
		atuEntry.ATUByte5 = aux;
*/
		atuEntry.EntryState_SPID = portVector==0 ? 0 : 7;
		atuEntry.PortVec = portVector;
		atuEntry.FID = 0;
		atuAddEntry(&atuEntry);
	}
}

void atuIgmpChange(int s) {
	atuIgmpAdd(s);
//	printf("Changed entry for service %d\n", s);
//	igmpServiceInfo(s);
}


void atuIgmpWrite() {
	int i;

//	printf(" ATU Igmp Write ");
//	printf("----------------");

//	atuFlush();
//	write("=================");
	for(i=0; i<IGMP_NUM_SERVICES; i++) {
		if(igmpService[i].valid) {
			atuIgmpAdd(i);
		}
	}
//	write("=================");
}

void atuTimer() {
	static unsigned char cnt = 0;
	ATU_ENTRY atuEntry;

	cnt++;
	if(cnt>=ATUPERIOD) {
		cnt = 0;

		memset(&atuEntry, 0, sizeof(ATU_ENTRY));

		// Go to the begining
		while(atuEntry.ATUByte0 != 0xFF &&
			atuEntry.ATUByte1 != 0xFF &&
			atuEntry.ATUByte2 != 0xFF &&
			atuEntry.ATUByte3 != 0xFF &&
			atuEntry.ATUByte4 != 0xFF &&
			atuEntry.ATUByte5 != 0xFF)
		{
			atuGetEntryNext(&atuEntry);
		}

		memset(&atuEntry, 0, sizeof(ATU_ENTRY));

		while(atuEntry.ATUByte0 != 0xFF &&
			atuEntry.ATUByte1 != 0xFF &&
			atuEntry.ATUByte2 != 0xFF &&
			atuEntry.ATUByte3 != 0xFF &&
			atuEntry.ATUByte4 != 0xFF &&
			atuEntry.ATUByte5 != 0xFF)
		{
			atuGetEntryNext(&atuEntry);
			if(atuEntry.ATUByte0 == 0x01 &
			   atuEntry.ATUByte1 == 0x00 &
			   atuEntry.ATUByte2 == 0x5E ) {
				// Find service
				struct eth_addr eth;
				eth.b0 = atuEntry.ATUByte0;
				eth.b1 = atuEntry.ATUByte1;
				eth.b2 = atuEntry.ATUByte2;
				eth.b3 = atuEntry.ATUByte3;
				eth.b4 = atuEntry.ATUByte4;
				eth.b5 = atuEntry.ATUByte5;

				// Check if there are ATU entries without service, and clean it.
				if(igmpHowMany(&eth,-1)==0) {
//					printf("Cleaning atu\n");
//					atuPrint(&atuEntry);
//					igmpInfo();

					atuEntry.EntryState_SPID = 0;
					atuEntry.PortVec = 0;
					atuEntry.FID = 0;
					atuAddEntry(&atuEntry);
				}
			}
		}

		// Check if all IGMP services has an ATU entry
		atuIgmpWrite();
	}
}
