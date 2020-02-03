#!/bin/bash

all:
	make clean
	make compile

compile: src/basic.c
	gcc src/basic.c -o main

clean:
	if [ -e main ]; then rm main; fi
