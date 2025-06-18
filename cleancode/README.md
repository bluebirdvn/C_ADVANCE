# Ứng Dụng Quản Lý Thư Viện Đơn Giản

Đây là một ứng dụng quản lý thư viện được viết bằng ngôn ngữ C, cho phép quản lý sách và người dùng, cũng như các hoạt động mượn/trả sách cơ bản.

## Mục Lục

- [Tính Năng Chính](#tính-năng-chính)
- [Cấu Trúc Dự Án](#cấu-trúc-dự-án)
- [Cách Biên Dịch và Chạy](#cách-biên-dịch-và-chạy)
- [Cách Sử Dụng](#cách-sử-dụng)
- [Mã Lỗi (Error Codes)](#mã-lỗi-error-codes)

## Tính Năng Chính

Ứng dụng cung cấp các chức năng sau:

### Quản Lý Sách
- Thêm sách mới với tiêu đề và tác giả.
- Chỉnh sửa thông tin tiêu đề và tác giả của sách hiện có.
- Xóa sách (chỉ khi sách không đang được mượn).
- Tìm kiếm sách theo tiêu đề (tìm kiếm theo chuỗi con).
- Tìm kiếm sách theo tác giả (tìm kiếm theo chuỗi con).
- Hiển thị danh sách tất cả các sách đang có sẵn.

### Quản Lý Người Dùng
- Thêm người dùng mới với tên.
- Chỉnh sửa tên người dùng hiện có.
- Xóa người dùng (chỉ khi người dùng không đang mượn bất kỳ cuốn sách nào).
- Hiển thị danh sách tất cả người dùng và các sách họ đang mượn.

### Quản Lý Mượn/Trả Sách
- Cho phép người dùng mượn sách có sẵn.
- Cho phép người dùng trả sách đã mượn.

## Cấu Trúc Dự Án

Dự án được tổ chức thành nhiều file `.h` (header) và `.c` (source) để modular hóa code:

- `main.c`: Chứa hàm `main`, khởi tạo hệ thống thư viện, thêm dữ liệu mẫu và vòng lặp chính của menu người dùng.
- `libdefines.h`: Định nghĩa các hằng số và kiểu liệt kê (enum) được sử dụng trong toàn bộ dự án (ví dụ: `MAX_BOOKS`, `BOOK_TITLE_LENGTH`, `BookStatus`, các mã lỗi).
- `bookmanagement.h` / `bookmanagement.c`:
    - `BookManagement` struct: Đại diện cho một cuốn sách, bao gồm ID, tiêu đề, tác giả và trạng thái.
    - Các hàm: `init_book` (khởi tạo sách), `display_book` (hiển thị thông tin sách).
- `usermanagement.h` / `usermanagement.c`:
    - `UserManagement` struct: Đại diện cho một người dùng, bao gồm ID, tên, danh sách ID sách đã mượn và số lượng sách đã mượn.
    - Các hàm: `init_user` (khởi tạo người dùng), `display_user` (hiển thị thông tin người dùng).
- `library.h` / `library.c`:
    - `Library` struct: Quản lý toàn bộ hệ thống thư viện, chứa mảng sách và người dùng, cùng với các bộ đếm và ID tiếp theo.
    - Các hàm quản lý chính:
        - `init_library`: Khởi tạo thư viện.
        - `add_book_to_library`, `add_user_to_library`: Thêm sách/người dùng.
        - `find_book_by_id`, `find_user_by_id`: Tìm kiếm sách/người dùng theo ID.
        - `display_available_books`, `display_all_users_and_borrowed_books`: Hiển thị danh sách.
        - `borrow_book_from_library`, `return_book_to_library`: Xử lý mượn/trả.
        - `edit_book_in_library`, `delete_book_from_library`: Chỉnh sửa/xóa sách.
        - `edit_user_in_library`, `delete_user_from_library`: Chỉnh sửa/xóa người dùng.
        - `find_book_by_title_substring`, `find_book_by_author_substring`: Tìm kiếm sách theo chuỗi con.
- `actionhandler.h` / `actionhandler.c`:
    - Các hàm tiện ích: `clear_input_buffer`, `get_string_input` (đọc chuỗi an toàn), `get_int_input` (đọc số nguyên an toàn).
    - Các hàm `handle_...`: Xử lý logic cho từng lựa chọn của người dùng từ menu chính, gọi các hàm tương ứng từ `library.c`.

## Cách Biên Dịch và Chạy

Để biên dịch và chạy ứng dụng này, bạn cần có một trình biên dịch C (ví dụ: GCC).

1.  **Mở Terminal hoặc Command Prompt.**

2.  **Điều hướng đến thư mục chứa các file mã nguồn của dự án.**
    ```bash
    cd du_an_quan_ly_thu_vien
    ```
    (Thay `du_an_quan_ly_thu_vien` bằng đường dẫn thực tế đến thư mục của bạn)

3.  **Biên dịch các file mã nguồn bằng GCC:**
    Chay lenh makefile
    Lệnh này sẽ biên dịch tất cả các file `.c` và tạo ra một file thực thi có tên `library_app` (hoặc `library_app.exe` trên Windows).

4.  **Chạy ứng dụng:**
    - Trên Linux/macOS:
        ```bash
        ./library_app
        ```
    - Trên Windows:
        ```bash
        library_app.exe
        ```

## Cách Sử Dụng

Khi chạy ứng dụng, bạn sẽ thấy một menu chính hiển thị các tùy chọn. Nhập số tương ứng với lựa chọn của bạn và nhấn Enter.
- 1-12: Các chức năng quản lý sách, người dùng và mượn/trả.
- 0: Thoát chương trình.

Ứng dụng sẽ yêu cầu bạn nhập thông tin cần thiết tùy thuộc vào chức năng bạn chọn.

## Mã Lỗi (Error Codes)

Dự án sử dụng các mã lỗi được định nghĩa trong `libdefines.h` để báo cáo trạng thái hoạt động:

- `CODE_SUCCESS`: 0 (Thành công)
- `CODE_ERROR_GENERIC`: -1 (Lỗi chung không xác định)
- `CODE_ERROR_NOT_FOUND`: -2 (Không tìm thấy đối tượng: sách, người dùng)
- `CODE_ERROR_ALREADY_EXISTS`: -3 (Đối tượng đã tồn tại - hiện chưa được sử dụng triệt để trong logic hiện tại)
- `CODE_ERROR_INVALID_INPUT`: -4 (Dữ liệu đầu vào không hợp lệ: rỗng, quá dài)
- `CODE_ERROR_CAPACITY_FULL`: -5 (Thư viện/mảng đầy)
- `CODE_ERROR_BOOK_NOT_AVAILABLE`: -6 (Sách không có sẵn hoặc đã được mượn)
- `CODE_ERROR_USER_MAX_BOOKS_REACHED`: -7 (Người dùng đã mượn tối đa số sách cho phép)
- `CODE_ERROR_BOOK_NOT_BORROWED_BY_USER`: -8 (Sách không được mượn bởi người dùng cụ thể)
- `CODE_ERROR_BOOK_BORROWED`: -9 (Sách đang được mượn và không thể xóa)