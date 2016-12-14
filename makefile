# Declaration of variables
CC       = clang++
CC_FLAGS   = -Wall

# File names
EXEC = fluffyforager
SOURCES  := $(wildcard *.cpp)

# Main target
$(EXEC): $(SOURCES)
	$(CC) $(CC_FLAGS) $(SOURCES) -o $(EXEC)
