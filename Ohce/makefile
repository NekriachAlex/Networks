TARGET = server

SOURCE = server.c

CC = gcc

CFLAGS = -Wall

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)