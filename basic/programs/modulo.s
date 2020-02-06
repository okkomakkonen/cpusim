; Calculate A mod n
  LDA %A
  LDB %n
loop:
  JPN %end
  SUB
  JMP %loop
end:
  ADD
  OUT
  HLT
A:
  237
n:
  17
