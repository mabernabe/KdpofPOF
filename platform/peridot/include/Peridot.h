#ifndef __PERIDOT_H__
#define __PERIDOT_H__
/**
 *  \file Peridot.h
 *  \brief Peridot Lib header file
 ********************************************************************************
 Marvell Commercial License Option
 If you received this File from Marvell and you have entered into a commercial
 license agreement (a "Commercial License") with Marvell, the File is licensed
 to you under the terms of the applicable Commercial License.
 ********************************************************************************
 */
#include <PeridotTypes.h>

// Lib Function

// Module SWReadWrite
unsigned short ReadReg(unsigned char dev, unsigned char reg);
void WriteReg(unsigned char dev, unsigned char reg, unsigned short data);
#define  PortRead(port, reg) ReadReg(port, reg)
#define  PortWrite(port, reg, data) WriteReg(port, reg, data)
#define  G1Read(reg) ReadReg(0x1b, reg)
#define  G2Read(reg) ReadReg(0x1c, reg)
#define  G1Write(reg, data) WriteReg(0x1b, reg, data)
#define  G2Write(reg, data) WriteReg(0x1c, reg, data)

// Module MDIOReadWrite
unsigned short C22RegRead(unsigned char PhyAddr, unsigned char Reg, unsigned char IsExternal);
void C22RegWrite(unsigned char PhyAddr, unsigned char Reg, unsigned short Data, unsigned char IsExternal);
unsigned short C45RegRead(unsigned char PhyAddr, unsigned char DevAddr, unsigned short Reg, unsigned char IsExternal);
void C45RegWrite(unsigned char PhyAddr, unsigned char DevAddr, unsigned short Reg, unsigned short Data, unsigned char IsExternal);
#define IntC22Read(PhyAddr, Reg) C22RegRead(PhyAddr, Reg, 0)
#define IntC22Write(PhyAddr, Reg, Data) C22RegWrite(PhyAddr, Reg, Data, 0)
#define IntC45Read(PhyAddr, dev, Reg) C45RegRead(PhyAddr, dev, Reg, 0)
#define IntC45Write(PhyAddr, dev, Reg, Data) C45RegWrite(PhyAddr, dev, Reg, Data, 0)
#ifndef GPIOSMI
#define ExtC22Read(PhyAddr, Reg) C22RegRead(PhyAddr, Reg, 1)
#define ExtC22Write(PhyAddr, Reg, Data) C22RegWrite(PhyAddr, Reg, Data, 1)
#define ExtC45Read(PhyAddr, dev, Reg) C45RegRead(PhyAddr, dev, Reg, 1)
#define ExtC45Write(PhyAddr, dev, Reg, Data) C45RegWrite(PhyAddr, dev, Reg, Data, 1)
#endif



// Module IO
void * FastCopy(void *dest,void *src,unsigned long count, unsigned char type);
#define memcpy_fast(dest, src, count) FastCopy(dest, src, count, 3)
#define eeprom2ram(dest, src, count)   FastCopy(dest, src, count, 1)

// Module TCAM
char TCAMOP(unsigned char index, char page, char op);
#define TCAMFlushAll()			TCAMOP(0, 0, 1)
char tcamLoadEntry(unsigned char tcamPointer, TCAM_DATA *tcamData);
char tcamReadTCAMData(unsigned char tcamPointer, TCAM_DATA *tcamData);
char tcamGetNextTCAMData(unsigned char *tcamPointer, TCAM_DATA *tcamData);
char tcamLoadEgrEntry(unsigned short tcamPointer, Egr_TCAM_DATA *data);

// Moulde VTU
void vtuFlush();
char vtuAddEntry(VTU_ENTRY *vtuEntry);
char vtuGetEntryNext(VTU_ENTRY  *vtuEntry);


// Moulde ATU
void atuFlush();
char atuAddEntry(ATU_ENTRY *atuEntry);
char atuGetEntryNext(ATU_ENTRY  *atuEntry);

// Moulde IRL
void irlInitialize();
char irlWrite(IRL_Entry *irlEntry);
char irlRead(IRL_Entry *irlEntry);

// Module Reset
void ResetSwitch();

// Lib++
#ifdef EIMPLIB
#include <Lib++.h>
#endif

#endif