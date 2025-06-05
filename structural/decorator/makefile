# Trình biên dịch và cờ biên dịch
CC = gcc
CFLAGS = -Wall -Iinc -Iinc/decorate

# Thư mục
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Tên chương trình
TARGET = $(BIN_DIR)/decorator_app

# Các file nguồn (chỉ trong src/)
SRC = $(wildcard $(SRC_DIR)/*.c)

# Tạo danh sách file .o tương ứng trong obj/
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Luật mặc định
all: $(TARGET)

# Build chương trình
$(TARGET): $(OBJ) main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_DIR)/main.o -o $@

# Build từng object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Tạo thư mục nếu chưa tồn tại
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
