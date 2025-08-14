# Linux System Monitor

A comprehensive real-time system monitoring application for Linux systems, written in C. This application provides detailed insights into CPU, memory, network, storage, and system information through an interactive terminal interface.

## Features

### 🖥️ System Overview
- **Summary Dashboard**: Quick overview of all system components
- **Real-time Updates**: Continuous monitoring with live data refresh
- **Interactive Navigation**: Easy-to-use menu system for detailed views

### 📊 Monitoring Capabilities

#### CPU Monitoring
- Overall CPU usage percentage
- Per-core CPU utilization
- CPU frequency monitoring
- CPU temperature (simulated)
- Top CPU-consuming process identification

#### Memory Management
- RAM usage statistics (total, used, free)
- Swap memory monitoring
- Buffer and cache information
- Memory usage percentages
- Top memory-consuming processes

#### Network Monitoring
- Network interface status (UP/DOWN)
- IPv4 and IPv6 address display
- Real-time data transfer speeds (RX/TX)
- Cumulative data transfer statistics
- TCP connection counts (total and established)
- Network utilization percentages

#### Storage Monitoring
- Disk capacity information (total, used, free)
- Storage device identification
- I/O statistics (bytes read/written)
- Storage performance benchmarking
- Mount point monitoring

#### System Information
- Operating system and kernel version
- System architecture
- System uptime
- Load averages (1, 5, 15 minutes)
- Process count
- Current date and time

## Project Structure

```
.
├── main.c                 # Main application entry point
├── Makefile              # Build configuration
├── model/                # Core monitoring modules
│   ├── model.c           # Main model coordination
│   ├── model.h           # Model interface definitions
│   ├── cpu/              # CPU monitoring module
│   │   ├── cpu.c
│   │   └── cpu.h
│   ├── memory/           # Memory monitoring module
│   │   ├── memory.c
│   │   └── memory.h
│   ├── network/          # Network monitoring module
│   │   ├── network.c
│   │   └── network.h
│   ├── storage/          # Storage monitoring module
│   │   ├── storage.c
│   │   ├── storage.h
│   │   └── Makefile
│   ├── system/           # System information module
│   │   ├── system.c
│   │   └── system.h
│   └── logger/           # Logging functionality
│       ├── logger.c
│       └── logger.h
└── README.md             # This file
```

## Architecture

### Design Patterns
- **Singleton Pattern**: Each monitoring module uses a singleton instance to ensure resource efficiency
- **Modular Design**: Each system component is encapsulated in its own module
- **Function Pointers**: Internal methods use function pointers for flexibility and extensibility

### Core Components

#### SystemMonitor
The main coordination structure that manages all monitoring modules:
- Initializes and coordinates all sub-modules
- Provides thread-safe data snapshots
- Manages the monitoring lifecycle

#### Module Structure
Each monitoring module follows a consistent pattern:
- **Manager Structure**: Contains all relevant data and function pointers
- **Create/Destroy**: Singleton lifecycle management
- **Update Functions**: Data collection and processing methods
- **Public API**: Safe interface for external access

## Building and Installation

### Prerequisites
- GCC compiler
- Linux operating system (tested on Ubuntu/Debian)
- Make utility
- Standard C libraries

### Build Instructions

1. **Clone or download the project**
   ```bash
   git clone <repository-url>
   cd linux-system-monitor
   ```

2. **Compile the application**
   ```bash
   make
   ```

3. **Run the application**
   ```bash
   ./system_monitor
   ```

### Build Targets
- `make` or `make all`: Build the complete application
- `make clean`: Remove compiled files
- `make debug`: Build with debug symbols

## Usage

### Starting the Application
```bash
./system_monitor
```

### Navigation Menu
The application provides an interactive menu system:

- **`m`** - Main summary view
- **`c`** - Detailed CPU information
- **`e`** - Memory details
- **`n`** - Network information
- **`s`** - Storage details
- **`y`** - System information
- **`q`** - Quit application

### Example Output

#### Summary View
```
===== Linux System Monitor =====
Time: 2024-01-15 14:30:25

--- System Summary ---
CPU Usage:  15.2%   | Temp: 45.3°C   | Freq: 2400 MHz
Mem Usage:  68.5%   | 10.95 / 16.00 GB
Disk Usage: 72.1%  | 650.5 / 902.3 GB (/)
Network RX:   12.50 kbps | TX:    8.25 kbps (eth0)
```

#### CPU Details
```
--- CPU Details ---
Overall Usage : 15.2%
Frequency     : 2400.50 MHz
Temperature   : 45.3 °C (Simulated)
Top Process   : firefox

Per-Core Usage:
  Core 0  : 18.5%
  Core 1  : 12.8%
  Core 2  : 16.1%
  Core 3  : 13.4%
```

## Technical Details

### Data Sources
The application reads system information from various Linux pseudo-filesystems:

- **`/proc/stat`**: CPU usage statistics
- **`/proc/meminfo`**: Memory information
- **`/proc/net/dev`**: Network interface statistics
- **`/proc/diskstats`**: Storage I/O statistics
- **`/sys/class/net/`**: Network interface details
- **`/proc/uptime`**: System uptime
- **`/proc/loadavg`**: Load averages

### Performance Considerations
- **Efficient Parsing**: Optimized file parsing for minimal overhead
- **Singleton Pattern**: Reduces memory allocation overhead
- **Selective Updates**: Each module can be updated independently
- **Simulated Data**: Some features (like CPU temperature) use simulated data for demonstration

### Memory Management
- Proper cleanup with destroy functions for all modules
- Safe memory allocation with error checking
- Prevention of memory leaks through careful resource management

## Configuration

### Customizable Parameters
- **Network Interface**: Automatically detects default interface or falls back to eth0
- **Storage Mount Point**: Monitors root filesystem by default
- **Update Intervals**: Built-in timing for data refresh
- **Display Precision**: Configurable decimal places for various metrics

### Logging
The application includes a comprehensive logging system:
- Multiple log levels (DEBUG, INFO, WARNING, ERROR, etc.)
- File and console output
- Timestamped entries
- Configurable log levels

## Limitations and Notes

1. **CPU Temperature**: Currently simulated (40-70°C range) as real temperature monitoring requires specific hardware access
2. **Storage Performance**: Benchmark tests are optional and may impact system performance
3. **Network Monitoring**: Focuses on primary network interface
4. **Permissions**: Some features may require specific permissions to access system files

## Troubleshooting

### Common Issues

**Application won't start**
- Ensure you have read permissions for `/proc` and `/sys` directories
- Check if the executable was built successfully

**Missing network data**
- Verify network interface name in `/proc/net/dev`
- Check if the interface is active

**Inaccurate disk usage**
- Ensure the mount point exists and is accessible
- Check permissions for filesystem access

### Debug Mode
Build with debug symbols for troubleshooting:
```bash
make debug
./system_monitor
```

## Future Enhancements

- **Real CPU Temperature**: Integration with hardware sensors
- **Historical Data**: Graphs and trends over time
- **Process Management**: Detailed process monitoring and control
- **Network Security**: Connection monitoring and analysis
- **Remote Monitoring**: Network-based monitoring capabilities
- **Configuration Files**: User-customizable settings
- **GUI Interface**: Graphical user interface option

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly on Linux systems
5. Submit a pull request

## License

This project is released under the MIT License. See LICENSE file for details.

## Authors

- System Architecture and Core Implementation
- Individual module development and optimization
- Documentation and testing

## Acknowledgments

- Linux kernel documentation for `/proc` and `/sys` filesystem details
- System programming best practices from the Linux community
- Performance optimization techniques for system monitoring applications

---

For questions, issues, or feature requests, please open an issue in the project repository.