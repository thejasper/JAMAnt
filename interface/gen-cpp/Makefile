# voor raspberry pi
#CC = g++-4.7 -O2 -std=c++11 -lthrift -lpthread -lboost_system -lboost_thread -DHAVE_CONFIG_H -DCONNECTED
#CFLAGS = -g -c -I/usr/local/include/thrift/
# voor arch linux (m'n laptop)
CC = g++ -O2 -std=c++11 -lthrift -lpthread -lboost_system -lboost_thread -DHAVE_CONFIG_H -DCONNECTED
CFLAGS = -g -c -I/usr/include/thrift/

all: antserver

antserver: Ant.o AntHandler.o ant_constants.o ant_types.o
	$(CC) Ant.o AntHandler.o ant_constants.o ant_types.o -o antserver

Ant.o: Ant.cpp 
	$(CC) $(CFLAGS) Ant.cpp

AntHandler.o: AntHandler.cpp rs232.h
	$(CC) $(CFLAGS) AntHandler.cpp

ant_constants.o: ant_constants.cpp
	$(CC) $(CFLAGS) ant_constants.cpp

ant_types.o: ant_types.cpp
	$(CC) $(CFLAGS) ant_types.cpp

clean:
	rm -rf *o antserver
