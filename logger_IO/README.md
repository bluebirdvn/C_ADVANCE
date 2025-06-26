# C Logging Module

## 📌 Giới thiệu

Đây là một **module ghi log (logger) tùy chỉnh bằng ngôn ngữ C** giúp theo dõi, gỡ lỗi và quản lý thông tin trong các ứng dụng một cách chuyên nghiệp. Module hỗ trợ:

- Ghi log ra **console** (stdout/stderr) và **file** đồng thời.
- **Lọc** log theo mức độ nghiêm trọng.
- **Tự động** thêm thời gian, tên file, và số dòng.
- Sử dụng **macro tiện lợi** để đơn giản hóa việc ghi log.

---


---

## ⚙️ Các mức log hỗ trợ

Module hỗ trợ 8 mức log tiêu chuẩn theo syslog:

| Mức log       | Giá trị enum | Ý nghĩa                            |
|---------------|--------------|-------------------------------------|
| LOG_EMERGENCY | 0            | Hệ thống không còn hoạt động       |
| LOG_ALERT     | 1            | Hành động cần thiết ngay lập tức   |
| LOG_CRITICAL  | 2            | Lỗi nghiêm trọng                   |
| LOG_ERROR     | 3            | Lỗi thường                        |
| LOG_WARNING   | 4            | Cảnh báo                          |
| LOG_NOTICE    | 5            | Thông báo quan trọng              |
| LOG_INFO      | 6            | Thông tin                         |
| LOG_DEBUG     | 7            | Thông tin gỡ lỗi chi tiết         |

---

## 📝 Định dạng log

Mỗi log có định dạng chuẩn:

[YYYY-MM-DD HH:MM:SS] [LEVEL] [FILENAME:LINE] - Message


---

## 🚀 Cách sử dụng

### 1. Khởi tạo logger

```c
int logger_init(const char* filename, log_level_t level);

filename: Đường dẫn file log (hoặc NULL để chỉ ghi console).

level: Mức log tối thiểu (ví dụ: LOG_INFO).

### 2. Ghi log

log_message(LOG_LEVEL, "Message format", ...);

### 3. Thay đổi mức log động

log_set_level(LOG_WARNING);

### 4. Kết thúc logger

logger_clean();

📚 Kỹ thuật sử dụng
stdarg.h: Xử lý tham số đầu vào thay đổi.

snprintf, vsnprintf: Định dạng chuỗi an toàn.

fopen, fprintf, fflush, fclose: Ghi file.

__FILE__, __LINE__: Macro biên dịch lấy metadata.

✅ Đặc điểm nổi bật
🔄 Đa đầu ra (stdout + stderr + file).

🧠 Tự động thêm metadata (thời gian, file, dòng).

🔍 Hỗ trợ lọc log theo cấp độ.

♻️ Dễ tái sử dụng trong các dự án C khác.

✅ Tuân thủ Clean Code: rõ ràng, đóng gói tốt.
