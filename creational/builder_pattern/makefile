SOURCE= main.c uartbuilder.c
TARGET= main
OBJS= $(SOURCE:.c=.o)
CFLAGS = -Wall -Wextra -std=c11
CC= gcc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean: 
	rm -rf $(OBJS) $(TARGET)
