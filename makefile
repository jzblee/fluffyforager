# Declaration of variables
CC       = g++
CC_FLAGS   = -Wall -g -m32

# File names
EXEC = fluffyforager
SOURCES  := $(wildcard *.cpp)

# Main target
$(EXEC): $(SOURCES)
	$(CC) $(CC_FLAGS) $(SOURCES) -o $(EXEC)
