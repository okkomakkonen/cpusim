; calculate fibonacci numbers
  LDA %zero
  LDB %one
loop:
  ADD
  JPC %end
; swap A and B
  ADD
  PSH
  SUB
  MAB
  POP
  SUB
  OUT
  JMP %loop
end:
  MBA
  OUT
  HLT
one:
  1
zero:
  0
