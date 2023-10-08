CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -pthread

SOURCES = main.cpp ConcurrentBuffer.cpp
HEADERS = ConcurrentBuffer.h

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp

.PHONY: clean

clean:
	rm -f main