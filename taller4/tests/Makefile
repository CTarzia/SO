CC=g++
CFLAGS=-g -Wall -pedantic -pthread

all: tests-drivers

tests-drivers: tests-drivers.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -std=c++0x

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@ -std=c++0x

clean:
	rm -rf *.o
	rm -rf tests-drivers
