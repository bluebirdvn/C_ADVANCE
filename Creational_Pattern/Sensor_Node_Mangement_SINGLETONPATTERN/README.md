# Sensor Manager App in C

## 📋 Description

This is a simple C application using the **Singleton** pattern to manage a list of sensor nodes, collect data from them, and store the results into a file.

## 🏗 Structure

- `SensorNode`: represents a single sensor with a unique ID.
- `SensorManager`: manages a list of sensors, collects data, and writes to file.
- Modular design: code is separated into `.h`/`.c` files.

## 📂 Folder Structure

.
├── inc/
├── src/
├── build/
├── Makefile
└── README.md


## ⚙️ Build & Run

```bash
make        # Build the project
make run    # Build and run
make clean  # Remove object files and binary

📄 Output

Data is written to sensorData.txt with timestamps and sensor values.
✅ Features

    Singleton pattern for SensorManager

    Modular design

    Simulated sensor data

    Easy to extend

Author: Tuan
