# Tên file output
TARGET = app

# Compiler
CC = gcc

# CFLAGS: bật cảnh báo, tiêu chuẩn C, debug symbols

CFLAGS = -Wall -Wextra -std=c11 -g -Ilogger
# File nguồn
SRCS = main.c logger/logger.c

# File đối tượng
OBJS = $(SRCS:.c=.o)

# Luật mặc định
all: $(TARGET)

# Luật build file chính
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Luật tạo .o từ .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dọn dẹp
clean:
	rm -f $(TARGET) $(OBJS)

# Tiện ích
run: all
	./$(TARGET)

