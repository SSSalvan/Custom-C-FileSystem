CC = gcc
CFLAGS = -Wall -Wextra
TARGET = main
OBJS = main.o abstract.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c abstract.h
	$(CC) $(CFLAGS) -c main.c

abstract.o: abstract.c abstract.h
	$(CC) $(CFLAGS) -c abstract.c

clean:
	rm -f $(TARGET) $(OBJS)
