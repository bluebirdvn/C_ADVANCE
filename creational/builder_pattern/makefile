CFLAGS = -Wall -Wextra -std=c11 -Iinc
CC= gcc

BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

TARGET = $(BIN_DIR)/main

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c = $(OBJ_DIR)/%.o)

all: createdir $(TARGET)
createdir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)



$(TARGET): $(OBJ) main.c
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_DIR)/main.o -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


clean: 
	rm -rf $(OBJ_DIR) $(BIN_DIR)
.PHONY: all clean