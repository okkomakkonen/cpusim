; initialize A register with one
  LDA %one
loop:
  ; add one to A register
  LDB %one
  ADD
  ; halt after A == 255
  JPC %halt
  ; push A to stack
  PSH
  JSR %is_prime
  ; check if A is prime
  LDB %one
  SUB
  ; output A if it's prime
  JPZ %out
  POP
  JMP %loop
out:
  POP
  OUT
  JMP %loop
halt:
  HLT
; return 1 if is prime else 0
is_prime:
  LDB %one
is_prime_loop:
  ; add one to B
  PSH
  MBA
  LDB %one
  ADD
  MAB
  POP
  ; test if A == B
  PSH
  SUB
  JPZ %prime ; jump if B == A
  POP
  ; test if A is divisible by B
  PSH
  JSR %mod
  JPZ %nprime
  POP
  JMP %is_prime_loop
prime:
  POP
  LDA %one
  RSR
nprime:
  POP
  LDA %zero
  RSR
; 0 in A register if divisible by B else 1
mod:
mod_loop:
  SUB
  JPN %mod_end
  JMP %mod_loop
mod_end:
  ADD ; A is now A mod B
  RSR
zero:
  0
one:
  1
