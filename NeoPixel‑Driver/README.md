# LED Strip Driver (WS2812B/NeoPixel) Example Project

Dự án này cung cấp một "driver" phần mềm mức cao mô phỏng thao tác điều khiển dải đèn LED RGB địa chỉ hóa như WS2812B (NeoPixel). Driver này giúp bạn quản lý buffer màu, thao tác từng pixel, và kiểm thử logic điều khiển LED trong môi trường C/C++.

## Tính năng

- **Quản lý bộ nhớ động** cho buffer màu của dải LED.
- **Đặt màu từng pixel** (theo chuẩn G-R-B của WS2812B).
- **Đổ màu toàn bộ dải LED** với một màu bất kỳ.
- **Tắt toàn bộ LED** (clear về màu đen).
- **API tiện ích**: lấy số pixel, lấy con trỏ buffer để kiểm thử.

## Cấu trúc mã nguồn

- `led_driver.h`  
  Định nghĩa các hàm API công khai để thao tác với dải LED.

- `led_driver.c`  
  Cài đặt chi tiết các hàm API, quản lý buffer động và thao tác bit cho màu sắc.

- `main.c`  
  Chương trình kiểm thử: mô phỏng các thao tác với dải LED, in ra giá trị màu từng pixel.

## Định dạng buffer màu

Mỗi pixel là một giá trị `uint32_t`, sử dụng 24 bit thấp:
- Bits 23-16: **Green**
- Bits 15-8: **Red**
- Bits 7-0: **Blue**
- Bits 31-24: Không sử dụng (luôn là 0)

Ví dụ:  
- Màu đỏ:    `0x0000FF00`  
- Màu xanh dương: `0x000000FF`  
- Màu trắng: `0x00FFFFFF`  
- Màu xanh lá: `0x00FF0000`  

## Hướng dẫn build & chạy

### Bước 1: Build

```sh
make
```

### Bước 2: Chạy

```sh
./led_test
```

### Kết quả mong đợi

```
Buffer khởi tạo về 0: OK
Pixel 0: 0x0000FF00 (expect 0x0000FF00)
Pixel 4: 0x00FFFFFF (expect 0x00FFFFFF)
Pixel 9: 0x000000FF (expect 0x000000FF)
Tất cả pixel màu xanh lá: OK
```

## API

```c
int led_init(size_t num_pixels);
void led_shutdown();
void led_set_pixel_color(size_t index, uint8_t r, uint8_t g, uint8_t b);
void led_fill(uint8_t r, uint8_t g, uint8_t b);
void led_clear();
const uint32_t* led_get_buffer();
size_t led_get_pixel_count();
```

## Ghi chú

- Dự án này chỉ mô phỏng logic điều khiển buffer dữ liệu màu, KHÔNG gửi tín hiệu vật lý ra dải LED thực tế.
- Có thể mở rộng để tích hợp với hàm truyền dữ liệu ra GPIO phù hợp với phần cứng thực tế.

---

**Tác giả:**  
- [bluebirdvn](https://github.com/bluebirdvn)