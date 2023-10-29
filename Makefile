CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -pthread

# Source files
SOURCES = src/main.cpp src/ConcurrentBuffer.hpp src/ThreadPool.hpp

# Output executable
OUTPUT = main

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES)

.PHONY: clean

clean:
	rm -f $(OUTPUT)
