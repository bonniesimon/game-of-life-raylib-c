# the compiler to use
CC = gcc

# compiler flags:
# -g flags for debugging
# -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -Wall

# additional libraries and flags (using raylib)
RAYLIB = $(shell pkg-config --libs --cflags raylib)

# the name to use for both the target source file and the output file
TARGET = src/main
EXE_DIR = dist/game_of_life

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(TARGET).c $(RAYLIB) -o $(EXE_DIR)
	./${EXE_DIR}

.PHONY: clean

clean:
	rm -f $(EXE_DIR)
