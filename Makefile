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
	rm -f $(OBJ) $(TARGET) pddl.c

# Run with valgrind
valgrind:
	valgrind --leak-check=full ./parser test/domain-snake.pddl test/problem-snake.pddl

# Run without valgrind
rsk:
	./parser test/domain-snake.pddl test/problem-snake.pddl
rni:
	./parser test/dni-snake.pddl test/pni-snake.pddl
rlo:
	./parser test/domain-lo.pddl test/problem-lo.pddl
rfa:
	./parser test/domain-lo-forall.pddl test/problem-lo-forall.pddl
rod:
	./parser test/domain-old.pddl test/problem-old.pddl
rtl:
	./parser test/domain-tele.pddl test/problem-tele.pddl
rfga:
	./parser test/domain-fga.pddl test/problem-fga.pddl
rcf:
	./parser test/domain-cafe.pddl test/problem-cafe.pddl
rmd:
	./parser test/domain-mercado.pddl test/problem-mercado.pddl
