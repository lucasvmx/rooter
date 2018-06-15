CC=gcc
WARNINGS=-Wall -Wextra
OPTIONS=-o rooter -static-libgcc -std=c11 -DDEBUG
DEFINES=
SOURCES=*.c
AUTOREVISION_OPTIONS=-t h
AUTOREVISION=autorevision
.PHONY: all revision

all: program

program: main.o
	$(CC) $(SOURCES) $(WARNINGS) $(OPTIONS) $(DEFINES)

revision:
	$(AUTOREVISION) $(AUTOREVISION_OPTIONS) > autorevision.h
