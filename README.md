# Design Patterns, System Simulation & Utility Modules in C/C++

## 📌 Giới thiệu

Đây là một repository tổng hợp nhiều **mẫu thiết kế phần mềm (Design Patterns)**, các **mô-đun tiện ích**, và một **mô phỏng hệ thống thực tế** được viết bằng ngôn ngữ C/C++. Mục tiêu là giúp bạn:

- Hiểu và áp dụng các **design patterns** vào thực tế.
- Sử dụng **logger, hệ quản lý thư viện** cho ứng dụng hệ thống.
- Triển khai các hệ thống mô phỏng có khả năng mở rộng, tổ chức tốt.

---

## Cấu trúc thư mục

| Thư mục                     | Mô tả ngắn                                                                 |
|-----------------------------|----------------------------------------------------------------------------|
| `Behavioral_Pattern`        | Cài đặt các mẫu hành vi như Observer, Command, State, ...       |
| `Creational_Pattern`        | Mẫu khởi tạo như Singleton, Factory Method,  Builder...  |
| `Structural_Pattern`        | Cài đặt các mẫu cấu trúc như  Decorator, Composite, ...   |
| `logger_IO`                 | Module ghi log tùy chỉnh (console + file), hỗ trợ nhiều mức độ log        |
| `Library_Management`        | Mô phỏng hệ thống quản lý thư viện sách (thêm, xoá, tìm kiếm, lưu file)   |
| `SPWS_SYSTEM_SIMULATION`    | Mô phỏng hệ thống nước thông minh (Smart Plant Watering System)           |
| `README.md`                 | Tài liệu hướng dẫn tổng quan repository                                   |

---

## Chi tiết các thành phần

### 1. Logger I/O (`logger_IO/`)
- Ghi log theo nhiều cấp độ: DEBUG, INFO, ERROR, ...
- Ghi đồng thời ra `stdout/stderr` và file `.log`
- Macro tiện lợi, có thể tái sử dụng cho các project C bất kỳ.

### 2. Library Management (`Library_Management/`)
- Quản lý sách, người mượn, lịch sử mượn/trả.
- Dữ liệu có thể lưu trữ và nạp lại từ file.
- Cấu trúc module rõ ràng (UI, data, xử lý).
- 
### 3. SPWS System Simulation (`SPWS_SYSTEM_SIMULATION/`)
- Mô phỏng hệ thống tưới cây thông minh.
- Bao gồm cảm biến ảo, phản hồi người dùng, và hành vi tự động.
- Có thể mở rộng để tích hợp với hệ thống IoT thật.

### 4. Design Patterns
#### ➤ `Behavioral_Pattern/`
- **Observer** – Theo dõi sự kiện.
- **Strategy** – Đổi thuật toán tại runtime.
- **Command** – Xử lý lệnh tách biệt khỏi logic.
- **State** – Chuyển trạng thái linh hoạt.

#### ➤ `Creational_Pattern/`
- **Singleton** – Đảm bảo một thể hiện duy nhất.
- **Factory Method** – Tạo đối tượng qua hàm ảo.
- **Builder** – Tách rời quy trình xây dựng đối tượng phức tạp.

#### ➤ `Structural_Pattern/`
- **Adapter** – Chuyển đổi interface.
- **Bridge** – Tách abstraction và implementation.
- **Composite** – Cấu trúc cây.
- **Decorator** – Thêm chức năng mà không sửa lớp gốc.

---

## Cách build & chạy

> Hầu hết module dùng CMake hoặc Makefile riêng trong thư mục con.

```bash
cd logger_IO
make
./main


Kiến thức áp dụng
Cấu trúc phần mềm với Design Patterns.

Lập trình hướng đối tượng và hướng mô-đun (C/C++)

Giao tiếp I/O, xử lý log, file, memory-safe.

Mô phỏng hệ thống nhúng/IoT đơn giản.

Tái sử dụng mã nguồn qua cấu trúc rõ ràng, linh hoạt.


Mục tiêu học tập
✔️ Hiểu và áp dụng các mẫu thiết kế chuẩn
✔️ Viết mã rõ ràng, mô-đun, tái sử dụng cao
✔️ Xây dựng các module tiện ích áp dụng được cho nhiều dự án C/C++ khác
✔️ Mô phỏng hệ thống thực tế từ ý tưởng đến thực thi
