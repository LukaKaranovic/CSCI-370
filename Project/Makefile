## CSCI 370: Makefile for 370 Project
# @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
# @version 1.0.0
# @date April 12, 2025

G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei
all: main viewermain

main: build/main.o build/DBObject.o build/managerfunc.o build/viewerfunc.o
	$(G) $^ -o main $(LFLAGS)

viewermain: build/viewermain.o build/DBObject.o build/viewerfunc.o
	$(G) $^ -o viewermain $(LFLAGS)

build:
	mkdir -p build

build/main.o: src/main.cpp src/DBObject.h | build
	$(G) $(CFLAGS) -c $< -o $@

build/viewermain.o: src/viewermain.cpp src/DBObject.h | build
	$(G) $(CFLAGS) -c $< -o $@

build/DBObject.o: src/DBObject.cpp src/DBObject.h | build
	$(G) $(CFLAGS) -c $< -o $@

build/managerfunc.o: src/managerfunc.cpp src/DBObject.h | build
	$(G) $(CFLAGS) -c $< -o $@

build/viewerfunc.o: src/viewerfunc.cpp src/DBObject.h | build
	$(G) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o main viewermain