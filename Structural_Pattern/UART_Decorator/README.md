# UART Communication Decorator Pattern Simulation

## 📚 Mô tả

Dự án này mô phỏng một hệ thống truyền thông UART sử dụng **Decorator Pattern** trong ngôn ngữ lập trình C.

Mục tiêu chính:
- Thiết kế module UART theo kiểu hướng đối tượng (OOP-style) trong C.
- Cho phép mở rộng các chức năng như **nén dữ liệu (RLE Compress)** và **kiểm tra CRC** mà không thay đổi lớp cơ sở UART.

## 🧱 Cấu trúc thư mục

├── inc/ # Thư mục chứa header files
│ ├── commonchannnel.h # Interface chung cho các channel
│ ├── decorate.h # Interface base decorator
│ └── decorate/ # Các decorator cụ thể
│ ├── compress.h
│ ├── crccheck.h
│ └── rawuartchannel.h
├── src/ # Thư mục chứa code triển khai
│ ├── compress.c
│ ├── crccheck.c
│ └── rawuartchannel.c
├── main.c # Chương trình chính để test kênh truyền
├── makefile # Tập tin build project
└── README.md 


## 🛠️ Cách build và chạy

```bash
make
./uart_decorator

🧪 Chức năng chính

    Raw UART Channel

        Cung cấp giao diện truyền dữ liệu cơ bản.

        Không áp dụng bất kỳ xử lý nào lên dữ liệu.

    Compress Decorator

        Sử dụng thuật toán Run Length Encoding (RLE) để nén dữ liệu trước khi gửi.

        Giải nén dữ liệu khi nhận.

    CRC Check Decorator

        Tính CRC cho dữ liệu trước khi gửi.

        Kiểm tra CRC khi nhận dữ liệu để đảm bảo tính toàn vẹn.

    Chuỗi Decorator

        RawUartChannel ← CompressDecorator ← CRCCheckDecorator

        Gửi dữ liệu qua chuỗi này và kiểm tra kết quả sau khi nhận.

📦 Ví dụ đầu ra (rút gọn)

Starting communication channel simulation...

1. Creating RawUartChannel (UART ID: 10)...
2. Creating Compress decorator, wrapping UART channel...
   Compress decorator created successfully.

3. Creating CrcCheck decorator, wrapping Compress channel...
--- Sending Data ---
Data sent successfully through the decorated channel.

--- Receiving Data ---
Attempting to receive data from the decorated channel...
Data received successfully (actual length: 10 bytes):
11 45 76 77 33 76 234 255 189 111
Verification: Received data matches original data. Success!

--- Cleaning up allocated memory ---
Destroyed CrcCheck decorator.
Destroyed Compress decorator.
Destroyed RawUartChannel.

Simulation finished.

🏗️ Design Pattern sử dụng

    Structural Pattern → Decorator: Cho phép "gói" nhiều chức năng mở rộng quanh một giao diện CommonChannel mà không cần sửa đổi lớp gốc RawUartChannel.