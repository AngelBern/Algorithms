#author Angel Delgado
#CSE 3318 Fall 2020


CFLAGS = -std=c99 -g -O0 -Wall

all: spell.c
	gcc -o hw5.out spell_checker.c spell.h spell.c ${CFLAGS}

test: all
	./hw5.out < testSmall.txt
clean:
	rm -f  hw4.out
