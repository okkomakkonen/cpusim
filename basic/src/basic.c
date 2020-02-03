// #include <stdlib.h> // fopen, fread
#include <stdint.h> // uint8_t
#include <stdio.h>  // printf

// 3 8-bit registers uint8_t
// Stack pointer, uint8_t
// Instruction pointer, uint8_t
// Flags register, uint8_t, ZF NF CF _ _ _ _ _

// 2*2^8 8-bit words of RW memory

// Flags
#define ZF 0x80  // zero flag
#define NF 0x40  // negative flag
#define CF 0x20  // carry flag

// Instructions
#define NOP 0x00 // no operation
#define HLT 0x01 // halt execution
#define LDA 0x02 // load next value to A register
#define ADD 0x03 // add next value to A register
#define SUB 0x04 // subtract next value from A register
#define STA 0x05 // store A register to memory
#define JMP 0x06 // jump to address
#define JPZ 0x07 // jump if zero
#define JPN 0x08 // jump if negative
#define JPC 0x09 // jump if carry

struct cpu_s {
  uint8_t a_reg;
  uint8_t b_reg;
  uint8_t c_reg;
  uint8_t stack_p;
  uint8_t instr_p;
  uint8_t flags;
};

void run_cpu(uint8_t mem[]) {

  int loop_counter = 0;

  // Initialize registers with 0
  struct cpu_s cpu = { 0 };

  while (1) {
    // Update loop counter, doesn't affect the cpu
    loop_counter++;

    // Fetch instruction at instr_p
    uint8_t instr = mem[cpu.instr_p];

    // Decode instruction, execute instruction
    if (instr == NOP) {
      cpu.instr_p++;
      continue;
    }
    if (instr == HLT) {
      break;
    }
    if (instr == ADD) {
      cpu.flags = 0;
      uint8_t a = mem[++cpu.instr_p];
      if (cpu.a_reg > 0xff - a) cpu.flags |= CF;
      cpu.a_reg += a;
      if (cpu.a_reg == 0) cpu.flags |= ZF;
      cpu.instr_p++;
      continue;
    }
    if (instr == SUB) {
      cpu.flags = 0;
      uint8_t a = mem[++cpu.instr_p];
      if (cpu.a_reg < a) cpu.flags |= NF;
      cpu.a_reg -= a;
      if (cpu.a_reg == 0) cpu.flags |= ZF;
      cpu.instr_p++;
      continue;
    }
    if (instr == LDA) {
      cpu.a_reg = mem[++cpu.instr_p];
      cpu.instr_p++;
      continue;
    }
    if (instr == JMP) {
      cpu.instr_p = mem[++cpu.instr_p];
      continue;
    }
    if (instr == JPN) {
      if (cpu.flags & NF) cpu.instr_p = mem[++cpu.instr_p];
      else cpu.instr_p++;
      continue;
    }
    if (instr == JPC) {
      if (cpu.flags & CF) cpu.instr_p = mem[++cpu.instr_p];
      else cpu.instr_p++;
      continue;
    }
    if (instr == JPZ) {
      if (cpu.flags & ZF) cpu.instr_p = mem[++cpu.instr_p];
      else cpu.instr_p++;
      continue;
    }

  }

  printf("Finished after %d operations\n", loop_counter);
  printf("Registers:\n");
  printf("A register: %d\n", cpu.a_reg);
  printf("B register: %d\n", cpu.b_reg);
  printf("C register: %d\n", cpu.c_reg);
  printf("Stack pointer: %d\n", cpu.stack_p);
  printf("Instruction pointer: %d\n", cpu.instr_p);
  printf("Flags: 0x%02x\n", cpu.flags);
  printf("ZF: %d, NF: %d, CF: %d\n", cpu.flags & ZF ? 1 : 0, cpu.flags & NF ? 1 : 0, cpu.flags & CF ? 1 : 0);

}

int main(int argc, char* argv[]) {

  uint8_t mem[2*256] = { LDA, 234, JPN, 8, SUB, 7, JMP, 2, ADD, 7, HLT };

  if (argc > 1) {
    FILE *f;
    f = fopen(argv[1], "rb");
    if (!f) {
      printf("Couldn\'t open file.\n");
      return 1;
    }

    if (fread(&mem, sizeof(uint8_t), 2*256, f) == 0) {
      printf("Couldn\'t read file.\n");
      return 1;
    }
  }

  run_cpu(mem);

  return 0;

}
