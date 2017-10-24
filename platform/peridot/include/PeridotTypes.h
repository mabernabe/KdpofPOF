#ifndef __TYPES_PERIDOT_H__
#define __TYPES_PERIDOT_H__
/*********************************************************************************
 Marvell Commercial License Option
 If you received this File from Marvell and you have entered into a commercial
 license agreement (a "Commercial License") with Marvell, the File is licensed
 to you under the terms of the applicable Commercial License.
*********************************************************************************/


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
    unsigned char SPV8_10:3;
    unsigned char res4:3;
    unsigned char FrameType:2;
    unsigned char MaskSPV8_10:3;
    unsigned char res5:3;
    unsigned char MaskType :2;

    unsigned char SPV0_7;
    unsigned char MaskSPV0_7;

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
    unsigned short VID :12;
    unsigned short VIDOverride :1;
    unsigned short IC :1;
    unsigned short Int :1;
    unsigned short C :1;
    //unsigned short d1;

    unsigned char FPRI:3;
    unsigned char FPRIOverride :1;
    unsigned char QPRI :3;
    unsigned char QPRIOverride :1;
    unsigned char NextIDFlowID;
    //unsigned short d2;

    unsigned char DPV:10;
    unsigned char SF :1;
    unsigned char res3: 4;
    //unsigned char QPRI_AVBOverride :1;
    //unsigned short d3;
    
    unsigned short res4;
    //unsigned short d4;

    unsigned char EgrActionPoint :6;
    unsigned char res5:2;
    unsigned char UnknownFiltering :2;
    unsigned char VTUPage :1;
    unsigned char VTUPageOverride :1;
    unsigned char ColorMode:2;
    unsigned char DPVMode :2;
    //unsigned short d5;

    unsigned char DSCP :6;
    unsigned char DSCPOverride :1;
    unsigned char res6 :1;
    unsigned short res7 :4;
    unsigned short LoadBlance :3;
    unsigned short LoadBlanceOverride :1;
    //unsigned short d6;

    unsigned short FAction : 15;
    unsigned short FActionOverride :1;
} PAGE2;

typedef struct {
    unsigned short EgrPort : 5;
    unsigned char res2;
    // d2

    unsigned short SAMode: 3;
    unsigned short res3: 1;
    unsigned short DAMode: 2;
    unsigned short res4: 2;
    unsigned short TagMode:2;
    unsigned short TagOverride:1;
    unsigned short res5: 1;
    unsigned short FrameMode: 2;
    unsigned short FrameOverride: 1;

    //d3

    unsigned short EgrVID: 12;
    unsigned short VIDMode: 2;
    unsigned short VIDOverride:1;

    //d4

    unsigned short EgrFPRI:3;
    unsigned short EgrCFI:1;
    unsigned short FPRIMode :2;
    unsigned short FPRIOverride:1;
    unsigned short EgrDSCP:6;
    unsigned short DSCPMode:2;
    // d5


} Egr_TCAM_DATA; //Egress page

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
    const unsigned char SPID:5;
    const unsigned char MissViolation :1;
    const unsigned char MemberViolation :1;
    const unsigned char Res2 :1;
    const unsigned char Res3 :2;
    const unsigned char VTUMode :2;
    unsigned char VTUOp :3;
    unsigned char VTUBusy:1;
    
    unsigned short VID :12;
    unsigned char Valid :1;
    unsigned char Page :1;
    const unsigned char Res03 :2;

    unsigned char MTagP0:2;
    unsigned char MTagP1:2;
    unsigned char MTagP2:2;
    unsigned char MTagP3:2;
    unsigned char MTagP4:2;
    unsigned char MTagP5:2;
    unsigned char MTagP6:2;
    unsigned char MTagP7:2;
    

    unsigned char MTagP8:2;
    unsigned char MTagP9:2;
    unsigned char MTagP10:2;
    const unsigned char Res13:2;
    unsigned char VIDFPri :3;
    unsigned char VIDFPriOverride :1;
    unsigned char VIDQPri :3;
    unsigned char VIDQPriOverride :1;

    // VTU FID
    unsigned char FID:12;
    //unsigned char Res20 :4;
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
    unsigned char MAC_FPri :3;
    const unsigned char Res0: 1;
    const unsigned char ATUFullViolation :1;
    const unsigned char MissViolation :1;
    const unsigned char MemberViolation :1;
    const unsigned char AgeOutViolation :1;
    unsigned char MAC_QPri :3;
    const unsigned char Res1 :1;
    unsigned char ATUOp :3;
    unsigned char ATUBusy :1;

    unsigned char EntryState_SPID :4;
    const unsigned char Res2 :4;
    const unsigned char Res3: 7;
    unsigned char LAG :1;

    unsigned char ATUByte1;
    unsigned char ATUByte0;
    unsigned char ATUByte3;
    unsigned char ATUByte2;
    unsigned char ATUByte5;
    unsigned char ATUByte4;

    
    
    // ATU FID
    unsigned short FID :12;


    // PortVec
    unsigned short PortVec:11;

} ATU_ENTRY;


typedef struct 
{
    // action 
    unsigned char IRLReg :4;
    const unsigned char Res7 :1;
    unsigned char IRLRes :3;
    unsigned char IRLPort: 5;
    unsigned char IRLOp :2;
    unsigned char IRLBusy :1;

    //0
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

    //1
    unsigned char PriSelect;
    unsigned short FPri :1;
    unsigned short PriAndPT :1;
    unsigned short AcctForAll :1;
    unsigned short AcctForQCong :1;
    unsigned short AcctForGrnOvflow :1;
    unsigned short ColorAware :1;
    unsigned short SMode :1;

    //2
    unsigned short BktInc : 13;
    unsigned short TCAMFlows :1;
    unsigned short  CountMode :2;

    //3
    unsigned short BktRateFactorGrn;

    // 4,5
    unsigned long CBSLimit; // only 24bit

    // 6
    unsigned short BktRateFactorYel;

    //7,8
    unsigned long EBSLimit;

    // 8short high
    unsigned char DaAvbNrlEn :1;
    unsigned char SaAvbNrlEn :1;
    unsigned char MGMTNrlEn :1;
    unsigned char FCAction :1;
    unsigned char FCMode :1;
    unsigned char FCPri :3;
   

}IRL_Entry;

#endif