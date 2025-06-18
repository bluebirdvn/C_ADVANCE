# 🌱 Smart Plant Watering System (SPWS)

## 📌 Giới thiệu

Đây là một **dự án mô phỏng hệ thống tưới cây thông minh** với chế độ **tự động và thủ công**, được thiết kế để hoạt động trên hệ thống nhúng (embedded systems). Dự án sử dụng **ngôn ngữ lập trình C**, áp dụng các **mẫu thiết kế hướng đối tượng** như:

- 🧩 **Observer** (Publisher-Subscriber)
- 🏭 **Factory** (sensor - sensor_factory)

Nhằm đạt được tính **module hóa**, **dễ mở rộng** và **dễ bảo trì**.

Hệ thống điều khiển bơm nước dựa trên dữ liệu độ ẩm từ cảm biến và cho phép người dùng **chuyển đổi linh hoạt giữa các chế độ hoạt động**, đồng thời cung cấp **log chi tiết với dấu thời gian** để theo dõi và gỡ lỗi.

---

## 🔧 Các Tính Năng Chính

### 🔁 Chế độ Tự động (Automatic Mode):

- Tự động bật bơm khi độ ẩm đất < `min_moisture`.
- Tự động tắt bơm khi:
  - Độ ẩm ≥ `max_moisture`, hoặc
  - Thời gian tưới > `max_watering_time_sec`.
- Đảm bảo khoảng thời gian chờ tối thiểu giữa các lần tưới (`MIN_INTERVAL_BETWEEN_WATERINGS_SEC`).

### 🤖 Chế độ Thủ công (Manual Mode):

- Người dùng có thể bật bơm thủ công trong `manual_watering_duration_sec`.
- Chỉ hoạt động khi hệ thống ở chế độ THỦ CÔNG và bơm đang tắt.
- Bơm tự tắt sau thời gian tưới thủ công đã cài.

### 🌡️ Mô phỏng Cảm biến Độ ẩm:

- Đọc dữ liệu ngẫu nhiên mô phỏng cảm biến vật lý.

### 💡 Mô phỏng Điều khiển Bơm và LED:

- Hiển thị trạng thái bơm và LED bằng thông báo console.

### 📋 Log hệ thống với dấu thời gian:

- Mọi sự kiện đều được ghi log với:
  - ⏱️ Dấu thời gian `YYYY-MM-DD HH:MM:SS`.
  - 🏷️ Mức độ log: `DEBUG`, `INFO`, `WARN`, `ERROR`.
  - 📝 Nội dung rõ ràng, có giá trị gỡ lỗi.

---

## 🧠 Thiết kế và Mẫu Thiết kế Sử Dụng

### 🔔 Observer (Publisher-Subscriber)
- Cảm biến là `Publisher`, thông báo cho `Subscriber` (như Pumping).
- Giúp tách rời xử lý sự kiện và luồng dữ liệu.

### 🏭 Factory
- Tạo cảm biến một cách linh hoạt.
- Dễ mở rộng với các loại cảm biến khác nhau.

### ⏳ Non-blocking
- Dùng dấu thời gian để quản lý logic thời gian (thay vì `sleep()`).
- Hữu ích trong môi trường nhúng hoặc yêu cầu real-time.

---

## 📁 Cấu Trúc Dự Án

├── main.c # Vòng lặp chính điều phối toàn hệ thống.
├── config.h/.c # Cấu hình hệ thống, định nghĩa cấu trúc, hằng số.
├── publisher.h/.c # Giao diện và logic cho Publisher.
├── subscriber.h/.c # Giao diện và xử lý khi nhận dữ liệu.
├── sensor.h # Giao diện cảm biến chung.
├── moisture_sensor.h/.c# Cảm biến độ ẩm (Publisher).
├── sensor_factory.h/.c # Factory tạo cảm biến.
├── pumping.h/.c # Điều khiển bơm + trạng thái LED (Subscriber).
├── modeswitch.h/.c # Logic điều khiển AUTO và MANUAL.
├── button.h/.c # Mô phỏng nút nhấn chuyển chế độ / kích hoạt tưới.
├── log.h/.c # Hệ thống log với dấu thời gian.
└── README.md # Tài liệu dự án này.

---

## 💻 Yêu Cầu Hệ Thống

- Trình biên dịch C hỗ trợ C11 (GCC khuyến nghị).
- Các thư viện chuẩn C: `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `stdint.h`.

---

## 🛠️ Hướng Dẫn Biên Dịch và Chạy

### ✅ Biên dịch:

Chạy file makefile.

▶️ Chạy chương trình:

./spws

🧪 Hướng Dẫn Sử Dụng

    Nhấn 'a' + Enter: Chuyển chế độ AUTO ↔ MANUAL.

    Nhấn 'm' + Enter: Kích hoạt tưới thủ công khi đang ở chế độ MANUAL.

    ⚠️ Lưu ý: vì là mô phỏng console, các lệnh scanf() hay getchar() sẽ chặn vòng lặp chính tạm thời. Trong hệ nhúng thực tế, cần dùng ngắt (interrupt) hoặc polling không chặn.

📤 Ví dụ Đầu ra Hệ thống

[2025-06-18 17:05:01] [INFO]  --- Smart Plant Watering System Started ---.
[2025-06-18 17:05:01] [INFO]  Moisture sensor object created successfully.
[2025-06-18 17:05:01] [DEBUG] Publisher initialized.
[2025-06-18 17:05:01] [INFO]  Moisture sensor init successful.
[2025-06-18 17:05:01] [DEBUG] Subscriber initialized.
[2025-06-18 17:05:01] [INFO]  Pumping system initialized.
[2025-06-18 17:05:01] [INFO]  Subscriber added to Publisher. Current count: 1.
[2025-06-18 17:05:01] [INFO]  Pumping subscribed to Moisture Sensor.
[2025-06-18 17:05:01] [INFO]  [BUTTONS] Initialized. (Simulated input: 'a' for mode toggle, 'm' for manual water).
[2025-06-18 17:05:01] [INFO]  Subscriber received event: MOISTURE=25.
[2025-06-18 17:05:01] [WARN]  [LED] State: LOW MOISTURE ALERT (Red), Moisture: 25% (Min: 30%).
[2025-06-18 17:05:01] [AUTO MODE] Current moisture: 25%. Min: 30%, Max: 70%.
[2025-06-18 17:05:01] [INFO]  [AUTO MODE] Moisture too low (25% < 30%), turning pump ON.
[2025-06-18 17:05:01] [INFO]  Pump turned ON.
[2025-06-18 17:05:01] [INFO]  [LED] State: WATERING (Yellow).

Press 'a' for AUTO/MANUAL toggle, 'm' for MANUAL water: a.
[2025-06-18 17:05:07] [INFO]  [MODE] Switched to MANUAL.
[2025-06-18 17:05:07] [INFO]  [LED] State: WATERING (Yellow).

Press 'a' for AUTO/MANUAL toggle, 'm' for MANUAL water: m.
[2025-06-18 17:05:10] [INFO]  [MANUAL MODE] Activating manual watering for 10 seconds.
[2025-06-18 17:05:10] [INFO]  Pump turned ON.
[2025-06-18 17:05:10] [INFO]  [LED] State: WATERING (Yellow).

Press 'a' for AUTO/MANUAL toggle, 'm' for MANUAL water: .
[2025-06-18 17:05:20] [INFO]  [MANUAL MODE] Manual watering time (10 sec) elapsed, turning pump OFF.
[2025-06-18 17:05:20] [INFO]  Pump turned OFF.
[2025-06-18 17:05:20] [INFO]  [AUTO MODE] Pump just turned OFF, setting last auto watering completion time.
[2025-06-18 17:05:20] [INFO]  [LED] State: NORMAL (Green).

📄 Giấy phép

    🔒 Dự án mang tính mô phỏng, không áp dụng trong sản phẩm thương mại thực tế nếu chưa được kiểm tra kỹ về tính an toàn và thời gian thực.


---



