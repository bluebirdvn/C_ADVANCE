📡 UART Configuration Builder Pattern in C
🧩 Giới thiệu

Dự án này triển khai Builder Design Pattern trong ngôn ngữ C để cấu hình UART một cách linh hoạt và rõ ràng. Mô hình này đặc biệt hữu ích khi có nhiều tùy chọn cấu hình (baudrate, parity, stop bits, data bits...), giúp tách biệt việc xây dựng cấu hình ra khỏi quá trình sử dụng.
📁 Cấu trúc thư mục

UART_Configuration_BUILDERPATTERN/
├── main.c                        # Chương trình chính, tạo và in cấu hình UART
├── makefile                      # File để biên dịch
├── README.md                     # Tài liệu hướng dẫn
├── inc/
│   └── uartbuilder.h             # Header định nghĩa builder và các enum cấu hình
└── src/
    └── uartbuilder.c             # Triển khai logic Builder Pattern

⚙️ Cấu hình UART hỗ trợ

    Baudrate: 1200 -> 400000 (mặc định: 9600)

    Parity: NONE, EVEN, ODD

    Stop bits: 1, 2

    Data bits: 5 -> 9 (mặc định: 8)

🧱 Builder Pattern API

UART_Builder builder = UART_Builder_Init();
UART_config_t* config = builder
    .setBaudrate(&builder, 115200)
    ->setParity(&builder, EVEN)
    ->setDataBits(&builder, 8)
    ->setStopBits(&builder, 1)
    ->build(&builder);

📦 UART_config_t gồm:

typedef struct {
    uint32_t baudrate;
    Parity parity;
    StopBits stopBits;
    DataBits dataBits;
} UART_config_t;

▶️ Cách biên dịch & chạy

Yêu cầu: gcc, make

make
./uart_app

Kết quả in ra:

UART Configuration:
Baudrate: 115200
Parity: EVEN
Stop bits: 1
Data bits: 8

🧼 Dọn dẹp

make clean
💡 Dự án mô phỏng cho mục đích học tập về Creational Design Pattern - Builder