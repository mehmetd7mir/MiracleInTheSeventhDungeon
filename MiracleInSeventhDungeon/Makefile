# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source and object files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXEC = $(BINDIR)/game

# Default target
all: $(EXEC)

# Link objects to create the executable
$(EXEC): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Run the program
run: $(EXEC)
	./$(EXEC)

# Phony targets
.PHONY: all clean run

