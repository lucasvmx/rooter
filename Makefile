CC=gcc
WARNINGS=-Wall -Wextra
OPTIONS=-o rooter -static-libgcc -std=c11
DEFINES=
SOURCES=main.c

all: main.o
	$(CC) $(SOURCES) $(WARNINGS) $(OPTIONS) $(DEFINES)
