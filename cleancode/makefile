TARGET = main

CC = gcc

CFLAGS = -Wall -Wextra -g -std=c11

SRCS = main.c bookmanagement.c library.c usermanagement.c
OBJS = $(SRCS:.c=.o)

DEPS = bookmanagement.h library.h usermanagement.h libdefines.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
