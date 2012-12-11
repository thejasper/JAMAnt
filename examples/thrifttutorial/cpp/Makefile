CC=g++ -O2 -lthrift -DHAVE_CONFIG_H
CFLAGS=-g -I./ -I/usr/include/thrift/ -c

all: CppClient CppServer

CppServer: tutorial_constants.o tutorial_types.o shared_constants.o shared_types.o Calculator.o SharedService.o CppServer.o 
	$(CC) CppServer.o Calculator.o tutorial_constants.o tutorial_types.o shared_types.o shared_constants.o SharedService.o -o CppServer

CppClient: tutorial_constants.o tutorial_types.o shared_constants.o shared_types.o Calculator.o SharedService.o CppClient.o
	$(CC) CppClient.o Calculator.o tutorial_constants.o tutorial_types.o shared_types.o shared_constants.o SharedService.o -o CppClient
	
tutorial_constants.o: tutorial_constants.cpp
	$(CC) $(CFLAGS) tutorial_constants.cpp

tutorial_types.o: tutorial_types.cpp
	$(CC) $(CFLAGS) tutorial_types.cpp

shared_constants.o: shared_constants.cpp
	$(CC) $(CFLAGS) shared_constants.cpp

SharedService.o: SharedService.cpp
	$(CC) $(CFLAGS) SharedService.cpp
	
shared_types.o: shared_types.cpp
	$(CC) $(CFLAGS) shared_types.cpp
	
Calculator.o: Calculator.cpp
	$(CC) $(CFLAGS) Calculator.cpp

CppServer.o: CppServer.cpp
	$(CC) $(CFLAGS) CppServer.cpp

CppClient.o: CppClient.cpp
	$(CC) $(CFLAGS) CppClient.cpp
	
clean:
	rm -rf *o calculator_server
