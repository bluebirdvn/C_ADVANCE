CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Tên file thực thi
TARGET = musicplayer

# Danh sách các file nguồn
SRCS = main.c \
       musicplayer.c \
       playingstate.c \
       pausingstate.c \
       stoppingstate.c

# Tạo danh sách file object tương ứng
OBJS = $(SRCS:.c=.o)

# Luật mặc định
all: $(TARGET)

# Link các object để tạo file thực thi
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Biên dịch từng file .c thành .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dọn dẹp các file build
clean:
	rm -f $(OBJS) $(TARGET)

# Tùy chọn tiện lợi
.PHONY: all clean
