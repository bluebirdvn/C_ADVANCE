CC = gcc
CFLAGS = -Wall -Wextra -Iinc -std=c99
OBJ_DIR = build

SRC_FILES = main.c src/sensor_node.c src/sensor_manager.c
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJ_TARGETS = $(addprefix $(OBJ_DIR)/, $(notdir $(OBJ_FILES)))

TARGET = sensor_app

all: $(TARGET)

$(TARGET): $(OBJ_TARGETS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: all
	./$(TARGET)
