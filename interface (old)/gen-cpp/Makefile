CC = g++ -O2 -std=c++11 -lthrift -lpthread -lboost_system -lboost_thread -DHAVE_CONFIG_H
CFLAGS = -g -c -I/usr/include/thrift/

all: antserver

antserver: Ant.o AntHandler.o TimeoutSerial.o ant_constants.o ant_types.o
	$(CC) Ant.o AntHandler.o TimeoutSerial.o ant_constants.o ant_types.o -o antserver

Ant.o: Ant.cpp 
	$(CC) $(CFLAGS) Ant.cpp

AntHandler.o: AntHandler.cpp 
	$(CC) $(CFLAGS) AntHandler.cpp

TimeoutSerial.o: TimeoutSerial.cpp
	$(CC) $(CFLAGS) TimeoutSerial.cpp

ant_constants.o: ant_constants.cpp
	$(CC) $(CFLAGS) ant_constants.cpp

ant_types.o: ant_types.cpp
	$(CC) $(CFLAGS) ant_types.cpp

clean:
	rm -rf *o antserver
