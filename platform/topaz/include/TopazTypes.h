/*********************************************************************************
 Marvell Commercial License Option
 If you received this File from Marvell and you have entered into a commercial
 license agreement (a "Commercial License") with Marvell, the File is licensed
 to you under the terms of the applicable Commercial License.
*********************************************************************************/
#ifndef __TYPES_TOPAZ_H__
#define __TYPES_TOPAZ_H__

typedef struct {
    unsigned char Data;
    unsigned char Mask;
} TCAMFRAMEOCTET;

typedef struct {
#if 0    
    unsigned short TCAMEntry:7;
    unsigned short res :2;
    unsigned short Page :2;
    unsigned short TCAMOp :3;
    unsigned short TB :1;
    
    unsigned short Reserved;
#endif
    const unsigned char res4:6;
    unsigned char FrameType:2;
    const unsigned char res5:6;
    unsigned char MaskType :2;

    unsigned char SPV:7;
    unsigned char MaskSPV;

    unsigned char PVIDH :4;
    unsigned char PPPRI :4;
	unsigned char MaskPVIDH :4;
    unsigned char MaskPPPRI :4;

    unsigned char PVIDL;
    unsigned char MaskPVIDL;

    TCAMFRAMEOCTET DA[6];
    TCAMFRAMEOCTET SA[6];
    TCAMFRAMEOCTET VLAN[4];
    TCAMFRAMEOCTET TYPE[2];
    TCAMFRAMEOCTET DATA[4];
} PAGE0;

typedef struct {
#if 0    
    unsigned short TCAMEntry:7;
    unsigned short res :2;
    unsigned short Page :2;
    unsigned short TCAMOp :3;
    unsigned short TB :1;
    
    unsigned short Reserved;
#endif
    TCAMFRAMEOCTET DATA[26];
} PAGE1;

typedef struct {
#if 0    
    unsigned short TCAMEntry:7;
    unsigned short res :2;
    unsigned short Page :2;
    unsigned short TCAMOp :3;
    unsigned short TB :1;
    //unsigned short d1;

    unsigned short Reserved;
#endif
    //unsigned short d0;    
    unsigned short VID :12;
    unsigned short VIDOverride :1;
    unsigned short IC :1;
    unsigned short Int :1;
    unsigned short C :1;
    //unsigned short d1;

    unsigned char FPRI:3;
    unsigned char FPRIOverride :1;
    unsigned char QPRI :2;
    const unsigned char res1 :1;
    unsigned char QPRIOverride :1;
    unsigned char NextIDFlowID;
    //unsigned short d2;

    unsigned char DPV:7;
    const unsigned char res2: 3;
    unsigned char SF :1;
    //unsigned char QPRI_AVB :2;
    const unsigned char res3: 4;
    //unsigned char QPRI_AVBOverride :1;
    //unsigned short d3;
    
    const unsigned short res4;
    //unsigned short d4;

    const unsigned char res5;
    unsigned char UnknownFiltering :2;
    const unsigned char res6 :4;
    unsigned char DPVMode :2;
    //unsigned short d5;

    const unsigned short res7 :12;
    unsigned short LoadBlance :3;
    unsigned short LoadBlanceOverride :1;
    //unsigned short d6;

    unsigned short FAction : 15;
    unsigned short FActionOverride :1;
} PAGE2;


typedef struct {
    PAGE0 page0;
    PAGE1 page1;
    PAGE2 page2;
} TCAM_DATA;


// vtu
#define MEMBER_EGRESS_UNMODIFIED    0
#define MEMBER_EGRESS_UNTAGGED      1
#define MEMBER_EGRESS_TAGGED        2
#define NOT_A_MEMBER                3

typedef struct
{
    unsigned char SPID:4;
    unsigned char Res1 :1;
    unsigned char MissViolation :1;
    unsigned char MemberViolation :1;
    const unsigned char Res2 :1;
    const unsigned char Res3 :4;
    unsigned char VTUOp :3;
    unsigned char VTUBusy:1;
    
    unsigned short VID :12;
    const unsigned char Valid :1;
    const unsigned char Res03 :3;

    unsigned char MTagP0:2;
    const unsigned char Res4:2;
    unsigned char MTagP1:2;
    const unsigned char Res5:2;
    unsigned char MTagP2:2;
    const unsigned char Res6:2;
    unsigned char MTagP3:2;
    const unsigned char Res7:2;

    unsigned char MTagP4:2;
    const unsigned char Res12:2;
    unsigned char MTagP5:2;
    const unsigned char Res13:2;
    unsigned char MTagP6:2;
    const unsigned char Res14:6;

    const unsigned short Res19 :12;
    unsigned char VIDPri :3;
    unsigned char VIDPRIOverride :1;

    // VTU FID
    unsigned char FID;
    const unsigned char Res20 :4;
    unsigned char VidPolicy:1;
    const unsigned char Res21 :3;
    // VTU SID
    unsigned char SID :6;
    const unsigned char Res22 :2;
    const unsigned char Res23 :4;
    unsigned char FilterMC :1;
    unsigned char FilterBC :1;
    unsigned char FilterUC :1;
    unsigned char DontLearn :1;

} VTU_ENTRY;

typedef struct
{
    const unsigned char Res0: 4;
    unsigned char ATUFullViolation :1;
    unsigned char MissViolation :1;
    unsigned char MemberViolation :1;
    unsigned char AgeOutViolation :1;
    unsigned char MACPri :3;
    const unsigned char Res1 :1;
    unsigned char ATUOp :3;
    unsigned char ATUBusy :1;

    unsigned char EntryState_SPID :4;
    unsigned char FromPort :4;
    unsigned char ToPort :4;
    const unsigned char Res2: 3;
    unsigned char LAG :1;

    unsigned char ATUByte1;
    unsigned char ATUByte0;
    unsigned char ATUByte3;
    unsigned char ATUByte2;
    unsigned char ATUByte5;
    unsigned char ATUByte4;

    // ATU FID
    unsigned char FID;

    // PortVec
    unsigned char PortVec;

} ATU_ENTRY;


typedef struct 
{
    // action 
    unsigned char IRLReg :4;
    unsigned char Res7 :1;
    unsigned char IRLRes :3;
    unsigned char IRLPort: 4;
    unsigned char IRLOp :3;
    unsigned char IRLBusy :1;

    unsigned short UnknownUnicastMask :1;
    unsigned short UnknownMulticastMask :1;
    unsigned short BroadcastMask :1;
    unsigned short MulticastMask :1;
    unsigned short UnicastMask :1;
    unsigned short MGMTFrames :1;
    const unsigned short Res0 :1;
    unsigned short ARP :1;
    unsigned short FLow0_TCPData :1;
    unsigned short FLow1_TCPCtrl :1;
    unsigned short FLow2_UDP :1;
    unsigned short FLow3_NONTCPUDP :1;
    unsigned short IMS :1;
    unsigned short PolicyMirror :1;
    unsigned short PolicyTrap :1;

    unsigned short BktInc : 12;

    unsigned short BktRateFactor;

    const unsigned char res1 :2;
    unsigned char CountMode :2;
    // CBSLimit
    unsigned char Res0_3 :4;
    unsigned char Res4_11 :8;
    
    // CBSLimit
    unsigned short Res12_23 :12;
    unsigned char  Res2 :4;

    //unsigned long EBSLimit:24;
    unsigned short Res_L16;

    // EBSLimit
    unsigned char Res_U8;
    const unsigned char Res3 :3;
    unsigned char SMode :1;
    unsigned char EBSLimitAction :1;
    unsigned char ActionMode :1;
    unsigned char IrIFCMode :1;
    const unsigned char Res4 :1;

    unsigned char DaAvbNrlEn :1;
    unsigned char SaAvbNrlEn :1;
    unsigned char MGMTNrlEn :1;
    unsigned char TCAMFlow :1;
    const unsigned char Res5: 3;
    unsigned char TSMMode :1;
    unsigned char PriMask :4;
    unsigned char PriOrPT :1;
    const unsigned char Res6 :1;
    unsigned char AcctForAll :1;
    unsigned char AcctForQCong :1;

    long CBSLimit;
    long EBSLimit;
    

}IRL_Entry;

#endif