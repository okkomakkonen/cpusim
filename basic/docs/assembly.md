# Instruction Set Architecture for basic CPU simulator

## Registers
register | size | description
---------|------|------------
a | 8 bits | general register
b | 8 bits | general register
pc | 8 bits | program counter
spc | 8 bits | subprogram counter
sp | 8 bits | stack pointer
flags | 8 bits | flags register
mar | 8 bits | memory address register
mbr | 8 bits | memory buffer register

## Flags
flags | description | value
------|-------------|------
ZF | zero flag | -
CF | carry flag | -
SF | sign flag | -
PF | parity flag | -

## Instructions
OPCODE | arguments | description | affected flags | hex value
-------|-----------|-------------|----------------|----------
NOP | no arguments | no operation | - | 0x01
HLT | no arguments | halt execution | - | 0x00
MOV | register, register | move value in second register to register | - | -
ADI | register, value | add value to register | CF, SF, ZF, PF | -
ADD | register, register | add value in second register to first register | CF, SF, ZF, PF | -
JMP | address | jump to memory address | - | -
JC | address | jump to memory address if carry flag is set | - | -
JZ | address | jump to memory address if zero flag is set | - | -
JN | address | jump to memory address if negative flag is set | - | -
ST | register, address | store value in register to memory address | - | -
JSR | address | jump to subroutine | - | -
RET | no arguments | return from subroutine | - | -
PSS | register | push value in register to stack | - | -
POS | register | pop value from stack to register | - | -
