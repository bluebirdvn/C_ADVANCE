CC = gcc
CFLAGS = -Wall -Iinc -Iinc/decorate

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/decorator_app

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: createdir $(TARGET)

$(TARGET): $(OBJ) main.c
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_DIR)/main.o -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

createdir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
