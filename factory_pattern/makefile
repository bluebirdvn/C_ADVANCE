CC = gcc
CFLAGS = -Wall -Wextra -Ihumidity -Itemperature -Ipressure -Ilight -IsensorFactory

SRC = main.c \
      humidity/humidity.c \
      temperature/temperature.c \
      pressure/pressure.c \
      light/light.c \
      sensorFactory/sensor_factory.c

OBJ = $(SRC:.c=.o)

TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
