CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
TARGET = main
SOURCE = main.c
DATA_FILE = sensorData.txt 


all: $(TARGET)


$(TARGET): $(SOURCE)
	@echo "Compiling $(SOURCE) -> $(TARGET)"
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo "Build complete: $(TARGET)"

clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) $(DATA_FILE)
	@echo "Cleanup complete."

.PHONY: all clean