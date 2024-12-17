# Davery Valdez
# Programming Assignment 1
# Fall 2024
# Purpose: Makefile

# Define variables
CC = gcc  # Compiler
CFLAGS = -Wall -g  # Compiler flags: -Wall enables all warnings, -g for debugging info
TARGET = email_sender  # The name of the final executable

# Default rule (runs when you just type 'make')
all: $(TARGET)

# Compile the source file(s) into the target executable
$(TARGET): email_sender.o
	$(CC) $(CFLAGS) -o $(TARGET) email_sender.o

# Compile the source file into an object file
email_sender.o: email_sender.c
	$(CC) $(CFLAGS) -c email_sender.c

# Clean rule to remove compiled files
clean:
	rm -f *.o $(TARGET)