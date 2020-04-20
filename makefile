# the compiler
CC = gcc

# compiler flags
# -g	add debugging info to the executable
# -Wall	turn on (most) compiler warnings
CFLAGS = -g -Wall

TARGET = main
OUTFILE = main.o

DATA_STRUCTS = ../data-struct/src
RELATIONAL_ALGEBRA = ../regex-parser/src
SRC = src

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(DATA_STRUCTS)/*.c $(RELATIONAL_ALGEBRA)/*.c $(SRC)/*.c $(TARGET).c -o $(OUTFILE)

do:
	$(CC) *.c -o $(OUTFILE)

run:
	./$(OUTFILE)

clean:
	$(RM) $(OUTFILE)
