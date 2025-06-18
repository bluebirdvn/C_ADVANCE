🌡️ Sensor Factory System in C
🧩 Mô tả

Dự án mô phỏng một hệ thống cảm biến sử dụng Factory Pattern để tạo và quản lý nhiều loại cảm biến khác nhau như:

    Nhiệt độ (temperature)

    Độ ẩm (humidity)

    Ánh sáng (light)

    Áp suất (pressure)

Mỗi cảm biến triển khai một giao diện chuẩn (Sensor) với các hàm khởi tạo và đọc dữ liệu.
📂 Cấu trúc thư mục

.
├── main.c
├── makefile
├── humidity/
│   ├── humidity.c
│   └── humidity.h
├── light/
│   ├── light.c
│   └── light.h
├── pressure/
│   ├── pressure.c
│   └── pressure.h
├── temperature/
│   ├── temperature.c
│   └── temperature.h
└── sensorFactory/
    ├── sensorfactory.c
    ├── sensorfactory.h
    └── sensor.h

🛠️ Build

Yêu cầu: gcc, make

Build chương trình:

make

Chạy chương trình:

./sensor_app

Clean (xóa file object và executable):

make clean

📌 Tính năng chính

    🎛️ Khởi tạo cảm biến theo yêu cầu thông qua createSensor(SENSOR_TYPE)

    📡 Giao diện Sensor chuẩn cho mọi loại cảm biến:

        init(): khởi tạo

        readData(): đọc dữ liệu giả lập (sinh ngẫu nhiên hoặc cố định)

    🏗️ Dễ dàng mở rộng thêm loại cảm biến mới

    📦 Quản lý bộ nhớ rõ ràng (malloc / free)

🔁 Factory Pattern Design

typedef struct {
    void (*init)(void);
    float (*readData)(void);
} Sensor;

typedef enum {
    SENSOR_TEMPERATURE,
    SENSOR_LIGHT,
    SENSOR_PRESSURE,
    SENSOR_HUMIDITY
} SensorType;

Sensor* createSensor(SensorType type);

📄 Ví dụ sử dụng

Sensor* humiditySensor = createSensor(SENSOR_HUMIDITY);
if (humiditySensor != NULL) {
    humiditySensor->init();
    printf("Humidity: %.2f\n", humiditySensor->readData());
    free(humiditySensor);
}
