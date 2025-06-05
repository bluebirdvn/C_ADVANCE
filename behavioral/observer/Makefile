# Compiler và flags
CC = gcc
CFLAGS = -Wall -Iinc -Iinc/sensors -Iinc/devices

# Thư mục
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Danh sách source files
SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(SRC_DIR)/sensors/*.c) \
      $(wildcard $(SRC_DIR)/devices/*.c)

# Tạo danh sách object file tương ứng
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Tên file thực thi
TARGET = $(BIN_DIR)/main

# Luật mặc định
all: $(TARGET)

# Tạo file thực thi
$(TARGET): $(OBJ) main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_DIR)/main.o -o $@

# Compile các file .c thành .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Tạo thư mục bin và obj nếu chưa có
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean: Xoá file object và thực thi
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
