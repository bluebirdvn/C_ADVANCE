# 🏡 Smart Home System - Facade Pattern in C

## 📘 Giới thiệu

Dự án này mô phỏng một **hệ thống nhà thông minh (Smart Home System)** sử dụng **Design Pattern: Facade** trong ngôn ngữ lập trình C.

Hệ thống điều khiển bao gồm:
- **HVAC System** (hệ thống điều hòa, sưởi)
- **Lighting System** (hệ thống chiếu sáng)
- **Security System** (hệ thống an ninh)

Thông qua một interface thống nhất - `SmartHomeSys` - người dùng có thể điều khiển toàn bộ hệ thống dễ dàng chỉ bằng các hàm facade như:
- `goodMorningMode()`
- `awayMode()`
- `movieMode()`
- `securityAlertOn()`, v.v.

---

## 🗂️ Cấu trúc thư mục

.
├── inc/ # Header files
│ ├── facade/facade.h
│ ├── hvacsystem/hvac.h
│ ├── lighting/lighting.h
│ └── security/security.h
├── lib/staticlib/ # Thư viện tĩnh (nếu có sử dụng)
│ ├── libmain.a
│ └── libmain.o
├── src/ # Code triển khai
│ ├── facade/facade.c
│ ├── hvacsystem/hvac.c
│ ├── lighting/lighting.c
│ └── security/security.c
├── main.c # Chương trình chính để test
├── makefile # Tập tin build project
└── README.md # File mô tả (file này)


---

## 🛠️ Cách biên dịch và chạy

```bash
make
./smarthome


🔧 Các chức năng chính
🧱 1. Facade: SmartHomeSys

Là lớp giao tiếp chính giúp điều phối các thành phần con trong hệ thống nhà thông minh.

typedef struct {
	HvacSystem *hvac;
	LightingSystem *light;
	SecuritySystem *security;
} SmartHomeSys;

💡 2. LightingSystem

    turnOn(), turnOff(), setBrightness()

❄️ 3. HvacSystem

    turnOn(), turnOff(), autoSet(), increaseTemp(), decreaseTemp()

🔐 4. SecuritySystem

    turnAlarmOn(), turnAlarmOff(), activeSecurity(), disableSecurity()

🧱 Áp dụng Design Pattern
✅ Facade Pattern

    Ẩn chi tiết phức tạp của nhiều subsystem bên dưới.

    Cung cấp một interface đơn giản để điều khiển tất cả các module nhà thông minh.

🖨️ Ví dụ đầu ra khi chạy chương trình

Set the light turning off.
Turning on air condition.
The security in active mode...
Good Morning Mode Entering...

Turning off air condition.
Set the light turning off.
The security in active mode...
Away Mode Mode Entering...

Get in auto setting mode.
Increase the brightness.
The security in disable mode...
Welcom Home Mode Entering...

Decreasing temperatur by -5.
Set the light turning off.
The security in active mode...
Movie Mode Entering...

Get in auto setting mode.
Increase the brightness.
The security in active mode...
Good night Mode Entering...

Turning off air condition.
Set the light turning on.
Turning on alarm...
Alert ON Mode Entering...

Increasing temperature by 5.
Increase the brightness.
Turning off alarm...
Alert OFF Mode Entering...

Destructor smartHomeSys.


