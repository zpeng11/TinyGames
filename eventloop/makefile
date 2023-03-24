CFLAGS = -c -Wall -o

all: Eventloop.cpp.o

Eventloop.cpp.o : Eventloop.cpp
	$(CC) $(CFLAGS) $@ $< 

clean:
	rm ./*.o
