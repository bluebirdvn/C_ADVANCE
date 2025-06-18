CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinc

SRC_DIR := src
BUILD_DIR := build
INCLUDES := -Iinc

SRC_FILES := $(wildcard $(SRC_DIR)/*.c) main.c
OBJ_FILES := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

all: spws

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

spws: $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) spws

.PHONY: all clean
