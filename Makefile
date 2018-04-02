CC=gcc
WARNINGS=-Wall -Wextra
OPTIONS=-o rooter -static-libgcc -std=c11
DEFINES=
SOURCES=main.c
AUTOREVISION=./autorevision -t h
.PHONY: all revision

all: program

program: main.o
	$(CC) $(SOURCES) $(WARNINGS) $(OPTIONS) $(DEFINES)

revision:
	$(AUTOREVISION) > autorevision.h
