# 🔔 Smart Sensor System - Observer Pattern in C

Dự án này mô phỏng một **hệ thống cảm biến thông minh** sử dụng **Observer Pattern** trong C. Các cảm biến như khói, nước, ánh sáng, cửa,... sẽ thông báo sự kiện đến các thiết bị đầu ra như **MobiApp**, **HVAC**, **Security**,... thông qua cơ chế publish-subscribe.

---

## 🎯 Mục tiêu

- Mô phỏng cơ chế publish-subscribe bằng cách sử dụng **Observer Pattern**.
- Cho phép nhiều thiết bị (subscriber) theo dõi cùng một cảm biến (publisher).
- Dễ dàng mở rộng hệ thống bằng cách thêm cảm biến hoặc thiết bị đầu ra mới.

---

## 📁 Cấu trúc thư mục

├── inc
│ ├── devices/ # Các thiết bị nhận thông báo
│ │ ├── hvacsys.h
│ │ ├── mobiapp.h
│ │ └── security.h
│ ├── publisher.h # Giao diện Publisher
│ ├── subscriber.h # Giao diện Subscriber
│ ├── sensors/ # Các cảm biến
│ │ ├── door.h
│ │ ├── light.h
│ │ ├── smoke.h
│ │ ├── temperature.h
│ │ └── water.h
├── src
│ ├── devices/
│ │ ├── hvacsys.c
│ │ ├── mobiapp.c
│ │ └── security.c
│ ├── sensors/
│ │ ├── door.c
│ │ ├── light.c
│ │ ├── smoke.c
│ │ ├── temperature.c
│ │ └── water.c
│ ├── publisher.c
│ └── subscriber.c
├── main.c
├── Makefile
└── README.md


---

## ⚙️ Biên dịch & Chạy chương trình

### 🔧 Biên dịch

```bash
make

▶️ Chạy chương trình

./smartsystem

🧪 Mô phỏng hoạt động

main.c tạo và khởi tạo các cảm biến và thiết bị:


LightSensor light;
LightSensorInit(&light);

DoorSensor door;
DoorSensorInit(&door);

SmokeSensor smoke;
SmokeSensorInit(&smoke);

WaterSensor water;
WaterSensorInit(&water);

MobiApp mobi;
MobiAppInit(&mobi);

light.base.subscribeToPub(&light.base, &mobi.base);
...
LightSensorTrigger(&light, 1); // Giả lập phát hiện ánh sáng
...
light.base.unSubscriberToPub(&light.base, &mobi.base);

Kết quả đầu ra (ví dụ):

Subscriber to PUB.
Subscriber to PUB.
...
Event: Light is ON
Event: Light is OFF
Event: Door is Open
Event: Smoke detected!
Event: Water leak detected!
Unsubscriber to PUB.


🧩 Design Pattern sử dụng
✅ Observer Pattern (Publish/Subscribe)

    Publisher giữ danh sách Subscriber.

    Khi sự kiện xảy ra, Publisher gọi notifyToSubscriber() để gửi thông báo đến tất cả các Subscriber.

    Các cảm biến kế thừa Publisher.

    Các thiết bị như MobiApp, HVACSys, SecuritySys là Subscriber.
   
📌 Các thành phần chính

| Thành phần    | Mô tả                                     |
| ------------- | ----------------------------------------- |
| `Publisher`   | Giao diện cảm biến, quản lý danh sách sub |
| `Subscriber`  | Giao diện nhận thông báo                  |
| `LightSensor` | Cảm biến ánh sáng (Publisher)             |
| `SmokeSensor` | Cảm biến khói                             |
| `DoorSensor`  | Cảm biến cửa                              |
| `WaterSensor` | Cảm biến rò rỉ nước                       |
| `MobiApp`     | Ứng dụng di động nhận thông báo           |
| `HVACSys`     | Hệ thống điều hòa nhận lệnh (chưa xử lý)  |
| `SecuritySys` | Hệ thống bảo mật (nâng cấp mở rộng)       |

🔌 Mở rộng

    Thêm cảm biến nhiệt độ, chuyển động,...

    Cho phép các thiết bị phản hồi lại sự kiện (HVAC mở máy lạnh nếu quá nóng).

    Giao tiếp mạng hoặc lưu log ra file.