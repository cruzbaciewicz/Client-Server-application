#!/bin/bash
#file: Makefile

CC = gcc
CFLAGS = -c -g -Wall 

all: server client


server: server.c
	$(CC) server.c -o server -lm

client: client.c
	$(CC) client.c -o client -lm

clean:
	rm -rf *.o server client	
