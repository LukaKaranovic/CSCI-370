## CSCI 370: Makefile for Assignment 8.
# @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
# @version 1.0.0
# @date March 18, 2025

G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei
all: a8

a8: a8.o functions.o
	$(G) $(CFLAGS) a8.o functions.o -o a8 $(LFLAGS)

a8.o: a8.cpp functions.h
	$(G) $(CFLAGS) -c a8.cpp -o a8.o

functions.o: functions.cpp functions.h
	$(G) $(CFLAGS) -c functions.cpp -o functions.o

clean:
	rm -f *.o a8