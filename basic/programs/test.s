; Calculates (A + B - C) and outputs it if it's less than or equal to zero
  NOP
  LDA %A
  LDB %B
  ADD
  LDB %C
  SUB
  JPZ %out
  JPN %out
  HLT
out:
  OUT
  HLT
A:
  42
B:
  96
C:
  139
