CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -pthread

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp

.PHONY: clean

clean:
	rm -f main