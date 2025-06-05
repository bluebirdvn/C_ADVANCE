# Trình biên dịch và cờ biên dịch
CC = gcc
CFLAGS = -Wall -Iinc -Iinc/state

# Thư mục
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Các file nguồn
SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(SRC_DIR)/state/*.c)

# Tên file thực thi
TARGET = $(BIN_DIR)/musicplayer

# Chuyển các file .c thành .o tương ứng trong obj/
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Luật mặc định
all: $(TARGET)

# Tạo file thực thi từ object và main.c
$(TARGET): $(OBJ) main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_DIR)/main.o -o $@

# Compile các file .c thành .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Tạo thư mục nếu chưa có
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean các file build
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
