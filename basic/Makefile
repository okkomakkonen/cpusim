#!/bin/bash

emulator: src/emulator.c
	gcc src/emulator.c -o main

simulator: src/simulator.c
	gcc src/simulator.c -o main

clean:
	if [ -e main ]; then rm main; fi
