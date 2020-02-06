#include <stdio.h>
#include <stdint.h> // uint8_t
#include <unistd.h> // usleep
#include "isa.h"

#define CLOCKS_PER_INSTR 6
#define CLOCK_SPEED 10      // Hz

// Status code is 16 bits
// First (MSB) 8 bits are opcode
// Next 3 bits are spc
// Last 5 bits are flags
// uint16_t status = opcode << 8 | spc << 5 | flags;

// microcode instructions
#define CI 0x1    // program counter in
#define CO 0x2    // program counter out
#define CE 0x4    // counter enable
#define AI 0x8    // A register in
#define AO 0x10   // A register out
#define BI 0x20   // B register in
#define BO 0x40   // B register out
#define MI 0x80   // memory address register in
#define MO 0x100  // memory buffer register out
#define II 0x200  // instruction register in
#define HT 0x400  // halt
#define RS 0x800  // reset subprogram counter
#define AL 0x1000 // AL register out
#define OT 0x2000 // output

struct cpu_s {
  uint8_t a_reg;   // A register
  uint8_t b_reg;   // B register
  uint8_t pc;      // program counter
  uint8_t mar;     // memory address register
  uint8_t mbr;     // memory buffer register
  uint8_t bus;     // common bus
  uint8_t spc;     // subprogram counter
  uint8_t flags;   // flags register
  uint8_t instr;   // instruction register
  uint8_t al_reg;  // alu register
};

void print_control_word(uint16_t control) {
  printf("Control: ");
  if (control & CI) printf("CI ");
  if (control & CO) printf("CO ");
  if (control & CE) printf("CE ");
  if (control & AI) printf("AI ");
  if (control & AO) printf("AO ");
  if (control & BI) printf("BI ");
  if (control & BO) printf("BO ");
  if (control & MI) printf("MI ");
  if (control & MO) printf("MO ");
  if (control & II) printf("II ");
  if (control & HT) printf("HT ");
  if (control & RS) printf("RS ");
  if (control & AL) printf("AL ");
  if (control & OT) printf("OT ");
  printf("| ");
}

void print_status(uint16_t status) {
  printf("Status: ");
  uint8_t opcode = (status & 0xff00) >> 8;
  if (opcode == NUL) printf("NUL ");
  if (opcode == HLT) printf("HLT ");
  if (opcode == NOP) printf("NOP ");
  if (opcode == LDA) printf("LDA ");
  if (opcode == LDB) printf("LDB ");
  if (opcode == JMP) printf("JMP ");
  if (opcode == ADD) printf("ADD ");
  if (opcode == SUB) printf("SUB ");
  if (opcode == XOR) printf("XOR ");
  if (opcode == OUT) printf("OUT ");
  if (opcode == JPZ) printf("JPZ ");
  if (opcode == JPN) printf("JPN ");
  if (opcode == JPC) printf("JPC ");
  printf("| ");

  uint8_t spc = (status & 0b11100000) >> 5;
  printf("%d | ", spc);

  uint8_t flags = (status & 0b11111);
  if (status & CF) printf("CF ");
  if (status & ZF) printf("ZF ");
  if (status & NF) printf("NF ");
  if (status & PF) printf("PF ");

}

void print_registers(struct cpu_s cpu) {

  printf("A: %d, ", cpu.a_reg);
  printf("B: %d | ", cpu.b_reg);

}

void init_microcode(uint16_t m[]) {

  // Fetch cycle is the same for all instructions
  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    for (uint16_t i = 0; i <= 0xff; i++) { // Loop through all instructions
      m[i<<8 | 0<<5 | f] = CO | MI;
      m[i<<8 | 1<<5 | f] = MO | II | CE;
    }
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[NOP<<8 | 2<<5 | f] = RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[HLT<<8 | 2<<5 | f] = RS | HT;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[LDA<<8 | 2<<5 | f] = CO | MI;
    m[LDA<<8 | 3<<5 | f] = MO | MI | CE;
    m[LDA<<8 | 4<<5 | f] = MO | AI | RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[LDB<<8 | 2<<5 | f] = CO | MI;
    m[LDB<<8 | 3<<5 | f] = MO | MI | CE;
    m[LDB<<8 | 4<<5 | f] = MO | BI | RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[JMP<<8 | 2<<5 | f] = CO | MI;
    m[JMP<<8 | 3<<5 | f] = MO | CI | RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[ADD<<8 | 2<<5 | f] = AL | AI | RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[SUB<<8 | 2<<5 | f] = AL | AI | RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[XOR<<8 | 2<<5 | f] = AL | AI | RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    m[OUT<<8 | 2<<5 | f] = OT | RS;
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    if (f & ZF) {
      m[JPZ<<8 | 2<<5 | f] = CO | MI;
      m[JPZ<<8 | 3<<5 | f] = MO | CI | RS;
    } else {
      m[JPZ<<8 | 2<<5 | f] = CE | RS;
    }
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    if (f & CF) {
      m[JPC<<8 | 2<<5 | f] = CO | MI;
      m[JPC<<8 | 3<<5 | f] = MO | CI | RS;
    } else {
      m[JPC<<8 | 2<<5 | f] = CE | RS;
    }
  }

  for (uint8_t f = 0; f < 0b100000; f++) { // Loop through all flags
    if (f & NF) {
      m[JPN<<8 | 2<<5 | f] = CO | MI;
      m[JPN<<8 | 3<<5 | f] = MO | CI | RS;
    } else {
      m[JPN<<8 | 2<<5 | f] = CE | RS;
    }
  }

}

void run_cpu(uint8_t mem[]) {

  int clock = 0;

  struct cpu_s cpu = { 0 };

  uint16_t microcode[(1<<16) * CLOCKS_PER_INSTR] = { 0 };
  init_microcode(microcode);

  uint16_t control = 0;
  uint16_t status = 0; // Might be a bad name

  while (1) {

    clock++;

    status = cpu.instr<<8 | cpu.spc<<5 | cpu.flags;
    control = microcode[status];

    // print_control_word(control);
    // print_registers(cpu);
    print_status(status);
    printf("\n");

    // subprogram counter
    cpu.spc = (cpu.spc + 1) % CLOCKS_PER_INSTR;

    // Set values on the bus
    if (control & CO) cpu.bus = cpu.pc;
    if (control & MO) cpu.bus = cpu.mbr;
    if (control & AO) cpu.bus = cpu.a_reg;
    if (control & BO) cpu.bus = cpu.b_reg;
    if (control & AL) cpu.bus = cpu.al_reg;

    // Take values from the bus
    if (control & CI) cpu.pc = cpu.bus;
    if (control & AI) cpu.a_reg = cpu.bus;
    if (control & BI) cpu.b_reg = cpu.bus;
    if (control & MI) cpu.mar = cpu.bus;
    if (control & II) cpu.instr = cpu.bus;

    // Utilities
    if (control & CE) cpu.pc++;
    if (control & RS) { cpu.spc = 0; cpu.instr = 0; }
    if (control & OT) printf("%d\n", cpu.a_reg);
    if (control & HT) break;

    // memory
    cpu.mbr = mem[cpu.mar];

    // alu
    if (cpu.instr == ADD) {
      cpu.flags = 0;
      if (cpu.a_reg > 0xff - cpu.b_reg) cpu.flags |= CF;
      cpu.al_reg = cpu.a_reg + cpu.b_reg;
      if (cpu.al_reg == 0) cpu.flags |= ZF;
    }
    if (cpu.instr == SUB) {
      cpu.flags = 0;
      if (cpu.a_reg < cpu.b_reg) cpu.flags |= NF;
      cpu.al_reg = cpu.a_reg - cpu.b_reg;
      if (cpu.al_reg == 0) cpu.flags |= ZF;
    }
    if (cpu.instr == XOR) {
      cpu.flags = 0;
      cpu.al_reg = cpu.a_reg ^ cpu.b_reg;
      if (cpu.al_reg == 0) cpu.flags |= ZF;
    }

    usleep(1e6 / CLOCK_SPEED);
  }

  printf("Finished after %d clock cycles\n", clock);
  printf("A register: %d\n", cpu.a_reg);
  printf("B register: %d\n", cpu.b_reg);

}

int main(int argc, char* argv[]) {

  uint8_t mem[2*256] = { NOP, JMP, 6, 42, 56, 34, LDA, 3, LDB, 4, ADD, LDB, 5, SUB, XOR, HLT };

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
