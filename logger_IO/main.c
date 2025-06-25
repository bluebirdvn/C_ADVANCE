#include "logger.h"
#include <stdio.h>

void connect_database() {
	log_message(LOG_INFO, "Đang thử kết nối tới cơ sở dữ liệu...");
	log_message(LOG_ERROR, "Kết nối thất bại: Không tìm thấy host 'db_server'.");
}

void process_data() {
	log_message(LOG_DEBUG, "Bắt đầu xử lý dữ liệu người dùng.");
	for (int i = 0; i < 3; i++) {
		log_message(LOG_DEBUG, "Đang xử lý bản ghi số %d.", i + 1);
	}
	log_message(LOG_INFO, "Xử lý dữ liệu thành công.");
}


int main() {
	if (logger_init("app.log", LOG_DEBUG) != 0) {
		return 1; // Thoát nếu không khởi tạo được logger
	}
	log_message(LOG_NOTICE, "Chương trình logger đã khởi động.");
	connect_database();
	process_data();
	log_message(LOG_WARNING, "Không gian đĩa sắp đầy (còn lại 10%%).");
	printf("\n--- THAY ĐỔI MỨC LOG SANG WARNING ---\n\n");
	log_set_level(LOG_WARNING);
	log_message(LOG_INFO, "Thông điệp này sẽ KHÔNG được hiển thị.");
	log_message(LOG_DEBUG, "Thông điệp gỡ lỗi này cũng sẽ KHÔNG được hiển thị.");
	log_message(LOG_ERROR, "Chỉ có lỗi hoặc các mức nghiêm trọng hơn được hiển thị.");
	log_message(LOG_CRITICAL, "Đây là một lỗi nghiêm trọng!");
	logger_clean();
        return 0;
}
