## CSCI 370: Makefile for Assignment 7.
# @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
# @version 1.0.0
# @date March 10, 2025

G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei
all: a7

a7: a7.cpp
	$(G) $(CFLAGS) $(LFLAGS) a7.cpp -o a7

clean:
	rm -f *.o a7