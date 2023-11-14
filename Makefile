CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -pthread -I/usr/local/Cellar/boost/1.83.0/include/boost # Include Boost headers
LIBS = -L/usr/local/Cellar/boost/1.83.0/lib -lboost_thread-mt -lboost_system-mt # Link against Boost libraries

# Source files
SOURCES = src/main.cpp 

# Output executable
OUTPUT = threadpool

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OUTPUT)
