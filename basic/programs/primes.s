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
  JPZ %is_prime_prime ; jump if B == A
  POP
  ; test if A is divisible by B
  PSH
  JSR %mod
  JPZ %is_not_prime
  POP
  JMP %is_prime_loop
is_prime_prime:
  POP
  LDA %one
  RSR
is_not_prime:
  POP
  LDA %zero
  RSR


; A mod B in A register, undefined behavior if B = 0
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
