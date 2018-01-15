#ifndef __IMP_IO__
#define __IMP_IO__

// CPU Configuration & Control
__sfr __at 0x00 EEPROM_BASE_SIZE;
__sfr __at 0x01 EEPROM_SPEED;
__sfr __at 0x02 CPU_CONTROL_0;
__sfr __at 0x03 CPU_CONTROL_1;
__sfr __at 0x04 COMM_STATUS;
__sfr __at 0x05 COMM_RD_DATA;
__sfr __at 0x06 COMM_WR_DATA;
__sfr __at 0x30 FAST_COPY_CTL;
__sfr __at 0x32 FAST_COPY_SIZE_LO;
__sfr __at 0x33 FAST_COPY_SIZE_HI;
__sfr __at 0x34 FAST_COPY_SRC_LO;
__sfr __at 0x35 FAST_COPY_SRC_HI;
__sfr __at 0x36 FAST_COPY_DST_LO;
__sfr __at 0x37 FAST_COPY_DST_HI;

// IMP Interrupt Control
__sfr __at 0x07 INT_SRC;
__sfr __at 0x08 INT_ACK_0_DATA;
__sfr __at 0x09 INT_ACK_1_DATA;
__sfr __at 0x0A INT_ACK_2_DATA;
__sfr __at 0x0B INT_ACK_3_DATA;
__sfr __at 0x0C INT_ACK_4_DATA;
__sfr __at 0x0D INT_ACK_5_DATA;
__sfr __at 0x0E INT_ACK_6_DATA;
__sfr __at 0x0F INT_ACK_7_DATA;

// Timer Registers
__sfr __at 0x10 TIMER0_CTL;
__sfr __at 0x11 TIMER1_CTL;
__sfr __at 0x13 WATCH_DOG;

__sfr __at 0x14 TIMER0_VAL_LO;
__sfr __at 0x15 TIMER0_VAL_HI;
__sfr __at 0x16 TIMER0_CNT_LO;
__sfr __at 0x17 TIMER0_CNT_HI;

__sfr __at 0x18 TIMER1_VAL_LO;
__sfr __at 0x19 TIMER1_VAL_HI;
__sfr __at 0x1A TIMER1_CNT_LO;
__sfr __at 0x1B TIMER1_CNT_HI;

// Ethernet NIC
__sfr __at 0x20 ETH_NIC_RX;
__sfr __at 0x22 ETH_RX_DATA;
__sfr __at 0x23 ETH_RX_SKIP;
__sfr __at 0x24 ETH_RX_OCTETS_LO;
__sfr __at 0x25 ETH_RX_OCTETS_HI;
__sfr __at 0x26 ETH_RX_ADDR_LO;
__sfr __at 0x27 ETH_RX_ADDR_HI;
__sfr __at 0x28 ETH_NIC_TX;
__sfr __at 0x2A ETH_TX_DATA;
__sfr __at 0x2B ETH_TX_PAD;
__sfr __at 0x2C ETH_TX_OCTETS_LO;
__sfr __at 0x2D ETH_TX_OCTETS_HI;
__sfr __at 0x2E ETH_TX_ADDR_LO;
__sfr __at 0x2F ETH_TX_ADDR_HI;

// Simulation environment (REG(0,3))
__sfr __at 0xFF CPU_MARKER;

#define REG(DEV_ADDR,REG_ADDR) (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2))
volatile __at REG(0,3) unsigned short CHIPID;
int add(int add1, int add2);

#endif
