main:
  LDA 18
  ADD 78
loop:
  JPZ %end
  SUB 96
  JMP %loop
end:
  HLT
