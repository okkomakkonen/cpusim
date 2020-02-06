# basic
This is a basic CPU simulator with a custom assembly language and an assembler.

## usage
Compile the CPU simulator with
```shell
make simulator
```
or
```shell
make emulator
```

Assemble a program with
```shell
mkdir bin
python3 assembler.py programs/modulo.s -o bin/modulo
```

Finally run the program with
```shell
./main bin/modulo
```
