# Instruction set architecture for a 8-bit CPU
The CPU has a data width of 8 bits and an address width of 16 bits.

## Registers
There are seven 8 bit registers A, B, C, D, E, H and L. The A register is the
accumulator. Registers BC, DE and HL can be used as 16 bit registers.

register | size | code
---------|------|-----
A | 8 bits | 0b000
B | 8 bits | 0b001
C | 8 bits | 0b010
D | 8 bits | 0b011
E | 8 bits | 0b100
H | 8 bits | 0b101
L | 8 bits | 0b110

If data is from or going to memory, it can be indicated with 'register' M = 0b111

## Instructions

opcode | arguments | description | binary
-------|-----------|-------------|--------
General instructions | | |
HLT | - | halts execution | 0b11111111
Moving data | | |
MOV | source register, destination register | moves data in source to destination | 0b10sssddd
MOV | memory address, destination register | moves data from memory address to destination | 0b10111ddd
