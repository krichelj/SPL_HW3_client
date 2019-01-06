CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib -lboost_system -pthread

all: BGSclient

BGSclient: bin/BGSclient.o bin/ConnectionHandler.o bin/ClientToServerEncoder.o bin/ServerToClientDecoder.o
	$(CC) -o bin/BGSclient bin/BGSclient.o bin/ConnectionHandler.o bin/ClientToServerEncoder.o bin/ServerToClientDecoder.o $(LFLAGS)

bin/BGSclient.o: src/BGSclient.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/BGSclient.o src/BGSclient.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/ClientToServerEncoder.o: src/ClientToServerEncoder.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ClientToServerEncoder.o src/ClientToServerEncoder.cpp

bin/ServerToClientDecoder.o: src/ServerToClientDecoder.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ServerToClientDecoder.o src/ServerToClientDecoder.cpp

clean: 
	rm -f bin/*
