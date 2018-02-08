#ifndef __IMP_DEFINE__
#define __IMP_DEFINE__

//-------------------------------------------
typedef unsigned int U16;
typedef unsigned char U8;
//-------------------------------------------

//-------------------------------------------
#define CPU_MARKER_PASS 0x50
#define CPU_MARKER_FAIL 0x46
//-------------------------------------------

// Write Memory Method 1
// ADDR: wirte address
// DATA: write data
#define WR_MEM(ADDR, DATA) \
        __asm \
        ld a, DATA \
        ld (ADDR), a \
        __endasm

// Write Memory Method 2
// write data is ready in A pair registers
// ADDR: wirte address
#define WR_MEM_EX(ADDR) \
        __asm \
        ld (ADDR), a \
        __endasm

// Read Memory
// ADDR: read address
#define RD_MEM(ADDR) \
        __asm \
        ld a, (ADDR) \
        __endasm

// Write Switch Register
// DEV_ADDR: device address
// REG_ADDR: register address
//     DATA: write data
#define WR_REG(DEV_ADDR, REG_ADDR, DATA) \
        __asm \
        ld hl, DATA \
        ld (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)), hl \
        __endasm

// Read Switch Register
// DEV_ADDR: device address
// REG_ADDR: register address
#define RD_REG(DEV_ADDR, REG_ADDR) \
        __asm \
        ld hl, (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        __endasm

// Check Switch Register Data
// DEV_ADDR: device address
// REG_ADDR: register address
//     DATA: check data
#define CHK_REG_DATA(DEV_ADDR, REG_ADDR, DATA) \
        __asm \
        ld bc, DATA \
        ld hl, (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        xor a, a \
        sbc hl, bc \
        call nz, _sim_fail \
        __endasm

// Check Switch Register Data (Low 8 bits)
// DEV_ADDR: device address
// REG_ADDR: register address
//     DATA: check data
#define CHK_REG_DATA_LO(DEV_ADDR, REG_ADDR, DATA) \
        __asm \
        ld a, (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        sub DATA \
        __endasm

// Check Switch Register Data (High 8 bits)
// DEV_ADDR: device address
// REG_ADDR: register address
//     DATA: check data
#define CHK_REG_DATA_HI(DEV_ADDR, REG_ADDR, DATA) \
        __asm \
        ld a, (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        ld a, (0xf001+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        sub DATA \
        __endasm

// Check Switch Register Bit (Low 8 bits)
// DEV_ADDR: device address
// REG_ADDR: register address
//   OFFSET: check bit OFFSET
#define CHK_REG_BIT_LO(DEV_ADDR, REG_ADDR, OFFSET) \
        __asm \
        ld a, (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        bit OFFSET, a \
        __endasm

// Check Switch Register Bit (High 8 bits)
// DEV_ADDR: device address
// REG_ADDR: register address
//   OFFSET: check bit OFFSET
#define CHK_REG_BIT_HI(DEV_ADDR, REG_ADDR, OFFSET) \
        __asm \
        ld a, (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        ld a, (0xf001+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        bit OFFSET, a \
        __endasm

// Read Higher 8 bits Switch Register
// DEV_ADDR: device address
// REG_ADDR: register address
#define RD_REG_HI(DEV_ADDR, REG_ADDR) \
        __asm \
        ld a, (0xf001+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        __endasm

// Read Lower 8 bits Switch Register
// DEV_ADDR: device address
// REG_ADDR: register address
#define RD_REG_LO(DEV_ADDR, REG_ADDR) \
        __asm \
        ld a, (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)) \
        __endasm

// Write Higher 8 bits Switch Register
// DEV_ADDR: device address
// REG_ADDR: register address
#define WR_REG_HI(DEV_ADDR, REG_ADDR, DATA) \
        __asm \
        ld a, DATA \
        ld (0xf001+(DEV_ADDR<<7)+(REG_ADDR<<2)), a \
        __endasm

// Write Lower 8 bits Switch Register
// DEV_ADDR: device address
// REG_ADDR: register address
#define WR_REG_LO(DEV_ADDR, REG_ADDR, DATA) \
        __asm \
        ld a, DATA \
        ld (0xf000+(DEV_ADDR<<7)+(REG_ADDR<<2)), a \
        __endasm

// Write IO Register
// IO_ADDR: IO register address
#define WR_IO(IO_ADDR) \
        __asm \
        out (IO_ADDR), a \
        __endasm

// Read IO Register
// IO_ADDR: IO register address
#define RD_IO(IO_ADDR) \
        __asm \
        in a, (IO_ADDR) \
        __endasm

// Check IO Register Bit
// IO_ADDR: IO register address
//  OFFSET: check bit OFFSET
#define CHK_IO_BIT(IO_ADDR, OFFSET) \
        __asm \
        in a, (IO_ADDR) \
        bit OFFSET, a \
        __endasm

// Check IO Register Data
// IO_ADDR: IO register address
//    DATA: compare data
#define CHK_IO_DATA(IO_ADDR, DATA) \
        __asm \
        in a, (IO_ADDR) \
        sub DATA \
        __endasm

// Set IO Register Bit
// IO_ADDR: IO register address
//  OFFSET: set bit OFFSET
#define SET_IO_BIT(IO_ADDR, OFFSET) \
        __asm \
        in a, (IO_ADDR) \
        set OFFSET, a \
        out (IO_ADDR), a \
        __endasm

// Clear IO Register Bit
// IO_ADDR: IO register address
//  OFFSET: clear bit OFFSET
#define CLR_IO_BIT(IO_ADDR, OFFSET) \
        __asm \
        in a, (IO_ADDR) \
        res OFFSET, a \
        out (IO_ADDR), a \
        __endasm

// Set Label
#define LABEL(label) \
        __asm \
        label: \
        __endasm

// Jump to Label
#define JUMP_TO_LABEL(label) \
        __asm \
        jp label \
        __endasm

// Jump to Label if Non-Zero
#define JUMP_TO_LABEL_NZ(label) \
        __asm \
        jp nz, label \
        __endasm

// Jump to Label if Zero
#define JUMP_TO_LABEL_Z(label) \
        __asm \
        jp z, label \
        __endasm

// NOP Instruction
#define NOP \
        __asm \
        nop \
        __endasm

// HALT Instruction
#define HALT while(1)
 #if 0 
   __asm \
        halt \
        __endasm
#endif 
// EI Instruction
#define EI \
        __asm \
        ei \
        __endasm

// DI Instruction
#define DI \
        __asm \
        di \
        __endasm

// EI in Mode 0
#define EI_0 \
        __asm \
        im 0 \
        ei \
        __endasm

// EI in Mode 1
#define EI_1 \
        __asm \
        im 1 \
        ei \
        __endasm

// EI in Mode 2
#define EI_2 \
        __asm \
        im 2    \
        ei      \
        __endasm

// Interrupt Table
//    IV: Interrupt Vector
// ADDR1: Interrupt Routine Address
// ADDR2: Interrupt Table Address
#define EI_2_TABLE(IV, ADDR1, ADDR2) \
        __asm \
        ld a, IV \
        ld i, a \
        ld hl, ADDR1 \
        ld ((IV << 8) + ADDR2), hl \
        __endasm

// Test Pass if Zero
#define SIM_PASS_Z \
        __asm \
        call z, _sim_pass \
        __endasm

// Test Pass if Non-Zero
#define SIM_PASS_NZ \
        __asm \
        call nz, _sim_pass \
        __endasm

// Test Fail if Zero
#define SIM_FAIL_Z \
        __asm \
        call z, _sim_fail \
        __endasm

// Test Fail if Non-Zero
#define SIM_FAIL_NZ \
        __asm \
        call nz, _sim_fail \
        __endasm

#endif