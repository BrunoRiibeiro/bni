# Compiler and flags
CC = gcc
CFLAGS = -Iinclude
DEBUG_FLAGS = -ggdb3

# Source and object files
SRC = parser.c linked_list.c stack.c symbol_table.c
OBJ = $(SRC:.c=.o)
TARGET = parser

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(DEBUG_FLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET) domain.c
