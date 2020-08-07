# 8086 ISA

The instruction set architecture for the Intel 8086.

Some references include
- [Wikipedia - 8086](https://en.wikipedia.org/wiki/Intel_8086)
- [Wikipedia - x86](https://en.wikipedia.org/wiki/X86#x86_registers)
- [Wikipedia - x86 Instructions](https://en.wikipedia.org/wiki/X86_instruction_listings#Original_8086/8088_instructions)

## Registers

The 8086 has 14 16-bit registers. The general purpose registers can be accessed by their low or high bits.

| Name | Size (bits) | Purpose |
| ---- | ------------|---------|
| AX | 16 | primary accumulator |
| BX | 16 | base |
| CX | 16 | counter |
| DX | 16 | accumulator |
| SP | 16 | stack pointer |
| BP | 16 | base pointer |
| SI | 16 | source index |
| DI | 16 | destination index |
| CS | 16 | code segment |
| DS | 16 | data segment |
| SS | 16 | stack segment |
| ES | 16 | extra segment |
| FLAGS | 16 | flags |
| IP | 16 | instruction pointer |
| AH | 8 | high of AX |
| AL | 8 | low of AX |
| BH | 8 | high of BX |
| BL | 8 | low of BX |
| CH | 8 | high of CX |
| CL | 8 | low of CX |
| DH | 8 | high of DX |
| DL | 8 | low of DX |

### Flags

| Flag | Meaning |
|------|---------|
| CF | carry |
| PF | parity |
| AF | auxiliary carry |
| ZF | zero |
| SF | sign |
| TF | trap |
| IF | interrupt |
| DF | direction |
| OF | overflow |

These flags are ordered in a status word in the following way.

| Bit  | 15-12 | 11 | 10 | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
|------|-------|----|----|----|----|----|----|----|----|----|----|----|----|
| Flag |       | OF | DF | IF | TF | SF | ZF |    | AF |    | PF |    | CF |
