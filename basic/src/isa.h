// Define Instruction Set Architecture for basic CPU simulator

// OPCODES
#define NUL 0x00 // empty
#define NOP 0x01 // no operation
#define LDA 0x02 // load to A register
#define LDB 0x03 // load to B register
#define JMP 0x04 // jump to address
#define ADD 0x05 // add A and B registers to A register
#define SUB 0x06 // subtract B register from A register
#define XOR 0x07 // xor A and B registers
#define NOT 0x0b // not A register
#define OUT 0x08 // output value in A register
#define JPZ 0x09 // jump if zero flag is set
#define HLT 0x0a // halt execution
#define JPN 0x0b // jump if negative flag is set
#define JPC 0x0c // jump if carry flag is set

// Flags
#define ZF 0x08  // zero flag
#define NF 0x04  // negative flag
#define CF 0x02  // carry flag
#define PF 0x01  // parity flag