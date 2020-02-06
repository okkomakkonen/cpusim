#include <stdint.h> // uint8_t
#include <stdio.h>  // printf
#include "isa.h"

struct cpu_s {
  uint8_t a_reg;
  uint8_t b_reg;
  uint8_t stack_p;
  uint8_t instr_p;
  uint8_t flags;
};

void run_cpu(uint8_t mem[]) {

  int loop_counter = 0;

  // Initialize registers with 0
  struct cpu_s cpu = { 0 };

  uint8_t instr;

  while (1) {
    // Update loop counter, doesn't affect the cpu
    loop_counter++;

    // Fetch instruction at instr_p
    instr = mem[cpu.instr_p];


    // Decode instruction, execute instruction
    if (instr == NOP) {
      cpu.instr_p++;
    }
    if (instr == HLT) {
      break;
    }
    if (instr == ADD) {
      cpu.flags = 0;
      if (cpu.a_reg > 0xff - cpu.b_reg) cpu.flags |= CF;
      cpu.a_reg += cpu.b_reg;
      if (cpu.a_reg == 0) cpu.flags |= ZF;
      cpu.instr_p++;
    }
    if (instr == SUB) {
      cpu.flags = 0;
      if (cpu.a_reg < cpu.b_reg) cpu.flags |= NF;
      cpu.a_reg -= cpu.b_reg;
      if (cpu.a_reg == 0) cpu.flags |= ZF;
      cpu.instr_p++;
    }
    if (instr == LDA) {
      cpu.a_reg = mem[mem[++cpu.instr_p]];
      cpu.instr_p++;
    }
    if (instr == LDB) {
      cpu.b_reg = mem[mem[++cpu.instr_p]];
      cpu.instr_p++;
    }
    if (instr == JMP) {
      cpu.instr_p = mem[mem[++cpu.instr_p]];
    }
    if (instr == JPN) {
      if (cpu.flags & NF) cpu.instr_p = mem[++cpu.instr_p];
      else cpu.instr_p += 2;
    }
    if (instr == JPC) {
      if (cpu.flags & CF) cpu.instr_p = mem[++cpu.instr_p];
      else cpu.instr_p += 2;
    }
    if (instr == JPZ) {
      if (cpu.flags & ZF) cpu.instr_p = mem[++cpu.instr_p];
      else cpu.instr_p += 2;
    }
    if (instr == XOR) {
      cpu.a_reg ^= cpu.b_reg;
      if (cpu.a_reg == 0) cpu.flags |= ZF;
      cpu.instr_p++;
    }
    if (instr == OUT) {
      printf("%d\n", cpu.a_reg);
      cpu.instr_p++;
    }

  }

  printf("Finished after %d operations\n", loop_counter);
  printf("Registers:\n");
  printf("A register: %d\n", cpu.a_reg);
  printf("B register: %d\n", cpu.b_reg);
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

    run_cpu(mem);

    fclose(f);
  } else {
    run_cpu(mem);
  }

  return 0;

}
