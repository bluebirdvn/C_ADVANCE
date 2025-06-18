
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -Iinc -Iinc/book -Iinc/library -Iinc/user

BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c) \
	   $(wildcard $(SRC_DIR)/book/*.c) \
	   $(wildcard $(SRC_DIR)/library/*.c) \
	   $(wildcard $(SRC_DIR)/user/*.c)

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = $(BIN_DIR)/main

all: createdir $(TARGET)
createdir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS) main.c
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(OBJ_DIR)/main.o -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)


