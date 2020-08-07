# basic
This is a basic CPU simulator with a custom assembly language and an assembler.

## usage
Compile the CPU simulator with
```shell
make simulator
```

Assemble a program with
```shell
mkdir bin
python3 assembler.py programs/primesmod6.s -o bin/primesmod6
```

Finally run the program with
```shell
./main bin/primesmod6
```
