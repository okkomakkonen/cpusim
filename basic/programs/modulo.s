  LDA 234   ; load 234 to A register
loop:
  JPN %end  ; jump to end if value is negative
  SUB 5     ; subtract 5 from A register
  JMP %loop ; jump to loop
end:
  ADD 5     ; add 5 to A register
  HLT       ; halt execution
