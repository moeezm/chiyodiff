# Define the compiler
CC = gcc

# Define compiler flags
CFLAGS = -O3 -Wall -I.

# Define linker flags
LDFLAGS = -lm

# Define the target executable
TARGET = chiyodiff

# Define the source files
SRCS = chiyodiff.c tensor.c A.c funcs.c

# Define the object files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build directory
clean:
	rm -f $(OBJS) $(TARGET)

