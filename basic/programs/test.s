; Calculates (A + B - C) ^ C
  NOP
  LDA %A
  LDB %B
  ADD
  LDB %C
  SUB
  XOR
  JMP %end
end:
  OUT
  HLT
A:
  42
B:
  96
C:
  45
