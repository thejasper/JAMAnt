OBJS = rs232.o ant.o main.o
CFLAGS = -c
CC = g++

main: $(OBJS)
	$(CC) $(OBJS) -o jamant

main.o: main.cpp ant.h
	$(CC) $(CFLAGS) main.cpp

ant.o: ant.cpp ant.h rs232.h
	$(CC) $(CFLAGS) ant.cpp

rs232.o: rs232.c rs232.h
	$(CC) $(CFLAGS) rs232.c

clean:
	rm -rf *.o jamant
