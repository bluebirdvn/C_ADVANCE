
CC := gcc
CFLAGS := -Wall -Iinc


TARGET := main


SRCS := main.c src/led_driver.c


OBJS := $(SRCS:.c=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
