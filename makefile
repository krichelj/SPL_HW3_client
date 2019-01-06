# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib -lboost_system -pthread

# All Targets
all: BGSclient

BGSclient: bin/BGSClient.o bin/ConnectionHandler.o bin/ClientToServerEncoder.o bin/ServerToClientDecoder.o
	@echo 'Building target: BGSclient'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/BGSClient bin/BGSClient.o bin/ConnectionHandler.o bin/ClientToServerEncoder.o bin/ServerToClientDecoder.o $(LFLAGS)
	@echo 'Finished building target: BGSclient'
	@echo ' '

bin/BGSClient.o: src/BGSClient.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/BGSClient.o src/BGSClient.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/ClientToServerEncoder.o: src/ClientToServerEncoder.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ClientToServerEncoder.o src/ClientToServerEncoder.cpp

bin/ServerToClientDecoder.o: src/ServerToClientDecoder.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ServerToClientDecoder.o src/ServerToClientDecoder.cpp

#Clean the build directory
clean: 
	rm -f bin/*
