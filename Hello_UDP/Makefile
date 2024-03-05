CC = gcc
CFLAGS = -Wall

SRCS = client1.c client2.c packages.c
OBJS = $(SRCS:.c=.o)
EXEC1 = client1
EXEC2 = client2

all: $(EXEC1) $(EXEC2)

$(EXEC1): client1.o packages.o
	$(CC) $(CFLAGS) $^ -o $@

$(EXEC2): client2.o packages.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c Packages.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC1) $(EXEC2)