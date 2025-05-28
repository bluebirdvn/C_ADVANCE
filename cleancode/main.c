#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "libdefines.h"
#include "bookmanagement.h"
#include "usermanagement.h"
#include "library.h"

/*
* Cac ham xu ly lua chon
*/
void handle_add_book (Library *lib);
void handle_edit_book (Library *lib);
void handle_delete_book (Library *lib);
void handle_search_books_by_title (Library *lib);
void handle_search_books_by_author (Library *lib);
void handle_display_available_books (const Library *lib); 
void handle_add_user (Library *lib);
void handle_edit_user (Library *lib);
void handle_delete_user (Library *lib);
void handle_display_all_users (const Library *lib); 
void handle_borrow_book (Library *lib);
void handle_return_book (Library *lib);


/*
* Cacs ham xu li tien ich dau vao
*/

/*
* ham xo bo dem dau vao (sau khi dung scanf hoac khi fgets de lai newline)
*/

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
/*
*Ham doc chuoi an toan 
* Tra ve 0 neu thanh cong, -1 neu rong
*/


int get_string_input(const char *prompt, char *buffer, int buffer_size) {
    if (prompt) {
        printf("%s", prompt);
    }
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        /*
        * xoa ky tu newline o cuoi neu co
        */
        buffer[strcspn(buffer, "\n")] = '\0';
        /*
        *Kiem tra xem chuoi co rong khong sau khi xoa newline
        */
        if (strlen(buffer) == 0) {
            
            return -1; 
        }
        return 0; 
    }
    /*
    * Xoa bo dem neu fgets loi
    */
    /*
    * Kiem tra eof
    */
    if (feof(stdin)) { 
        /*
        * xoa co loi tranh anh huong lan doc tiep theo
        */
        clearerr(stdin); 

        printf("\nKet thuc nhap lieu (EOF).\n");

    } else if (ferror(stdin)) {

        perror("Loi doc dau vao");

        clearerr(stdin);
    } else {
        /*
        * Truong hop buffer day va khong doc duoc newline
        */
        clear_input_buffer();
    }
    return -1;
}

/*
*Hàm đọc một số nguyên từ người dùng
*Trả về số nguyên nếu thành công, hoặc một giá trị lỗi (ví dụ -1) nếu không hợp lệ
*/

int get_int_input(const char *prompt) {
    char buffer[32];
    int value;
    char *endptr;

    while (1) {
        if (get_string_input(prompt, buffer, sizeof(buffer)) == 0) {
            if (strlen(buffer) == 0) { 
                printf("Loi: Vui long nhap mot so.\n");
                continue;
            }
            value = strtol(buffer, &endptr, 10);
            /*
            *Kiểm tra xem toàn bộ chuỗi đã được chuyển đổi và không có ký tự thừa
            *loại trừ khoảng trắng ở cuối nếu có
            */
            char *check_ptr = endptr;
            while (*check_ptr != '\0' && isspace((unsigned char)*check_ptr)) {
                check_ptr++;
            }

            if (*check_ptr == '\0') { 
                return value;
            } else {
                printf("Loi: '%s' khong phai la mot so nguyen hop le. Vui long nhap lai.\n", buffer);
            }
        } else {
            /*
            * get_string_input da xu li loi doc hoac eof, co the can thaot neu la eof
            */
            if (feof(stdin)) return -999; 
            printf("Loi: Nhap lieu khong hop le. Vui long nhap lai.\n");
        }
    }
}



void display_main_menu() {
    printf("\n============================================\n");
    printf("     UNG DUNG QUAN LY THU VIEN\n");
    printf("============================================\n");
    printf("--- Quan Ly Sach ---\n");
    printf("1. Them sach moi\n");
    printf("2. Chinh sua thong tin sach\n");
    printf("3. Xoa sach\n");
    printf("4. Tim kiem sach theo Tieu De\n");
    printf("5. Tim kiem sach theo Tac Gia\n");
    printf("6. Hien thi danh sach sach co san\n");
    printf("--- Quan Ly Nguoi Dung ---\n");
    printf("7. Them nguoi dung moi\n");
    printf("8. Chinh sua thong tin nguoi dung\n");
    printf("9. Xoa nguoi dung\n");
    printf("10. Hien thi tat ca nguoi dung va sach da muon\n");
    printf("--- Quan Ly Muon/Tra Sach ---\n");
    printf("11. Muon sach\n");
    printf("12. Tra sach\n");
    printf("--------------------------------------------\n");
    printf("0. Thoat chuong trinh\n");
    printf("============================================\n");
}


int main() {
    Library library_system;
    init_library(&library_system);
    int choice;

    /*
    * Them mot so mau tuy chon
    */
    add_book_to_library(&library_system, "Lap Trinh C", "Nguyen Van A");
    add_book_to_library(&library_system, "Cau Truc Du Lieu", "Tran Thi B");
    add_book_to_library(&library_system, "Clean Code", "Robert C. Martin");
    add_user_to_library(&library_system, "Alice");
    add_user_to_library(&library_system, "Bob");


    while (1) {
        display_main_menu();
        choice = get_int_input("Lua chon cua ban: ");
        if (choice == -999) { 
            printf("Thoat chuong trinh do EOF.\n");
            break;
        }


        switch (choice) {
            case 1:
                handle_add_book(&library_system);
                break;
            case 2:
                handle_edit_book(&library_system);
                break;
            case 3:
                handle_delete_book(&library_system);
                break;
            case 4:
                handle_search_books_by_title(&library_system);
                break;
            case 5:
                handle_search_books_by_author(&library_system);
                break;
            case 6:
                handle_display_available_books(&library_system);
                break;
            case 7:
                handle_add_user(&library_system);
                break;
            case 8:
                handle_edit_user(&library_system);
                break;
            case 9:
                handle_delete_user(&library_system);
                break;
            case 10:
                handle_display_all_users(&library_system);
                break;
            case 11:
                handle_borrow_book(&library_system);
                break;
            case 12:
                handle_return_book(&library_system);
                break;
            case 0:
                printf("Tam biet!\n");
                exit(0); 
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
        }
        printf("\nNhan Enter de tiep tuc...");

        /*
        *Don dep neu co enter thua tu lan nhap truoc
        */
        clear_input_buffer(); 
    }

    return 0;
}

/*
* Trien khai cac ham xu li
*/


void handle_add_book (Library *library) {
    char title[BOOK_TITLE_LENGTH];
    char author[AUTHOR_NAME_LENGTH];
    printf("\n--- Them Sach Moi ---\n");

    if (get_string_input("Nhap tieu de sach: ", title, sizeof(title)) != 0) {
        printf("Loi: Tieu de khong duoc de trong.\n"); return;
    }
    if (get_string_input("Nhap tac gia sach: ", author, sizeof(author)) != 0) {
        printf("Loi: Tac gia khong duoc de trong.\n"); return;
    }

    int result = add_book_to_library(library, title, author);

    if (result == CODE_SUCCESS) {
        printf("Them sach thanh cong!\n");
    } else if (result == CODE_ERROR_CAPACITY_FULL) {
        printf("Loi: Thu vien da day, khong the them sach.\n");
    } else if (result == CODE_ERROR_INVALID_INPUT) {
        printf("Loi: Tieu de hoac tac gia khong hop le.\n");
    }
    else {
        printf("Loi: Khong the them sach (Ma loi: %d).\n", result);
    }
}

void handle_edit_book (Library *library) {
    printf("\n--- Chinh Sua Thong Tin Sach ---\n");
    int book_id = get_int_input("Nhap ID sach can chinh sua: ");
    if (book_id == -999) return;


    BookManagement* book = find_book_by_id(library, book_id);
    if (!book) {
        printf("Loi: Khong tim thay sach voi ID %d.\n", book_id);
        return;
    }

    printf("Thong tin sach hien tai:\n");
    display_book(book);

    char new_title[BOOK_TITLE_LENGTH];
    char new_author[AUTHOR_NAME_LENGTH];

    printf("Nhap tieu de moi (de trong neu khong muon thay doi): ");
    get_string_input(NULL, new_title, sizeof(new_title)); 

    printf("Nhap tac gia moi (de trong neu khong muon thay doi): ");
    get_string_input(NULL, new_author, sizeof(new_author)); 

    if (strlen(new_title) == 0 && strlen(new_author) == 0) {
        printf("Khong co thong tin nao duoc thay doi.\n");
        return;
    }

    int result = edit_book_in_library(library, book_id, strlen(new_title) > 0 ? new_title : NULL, strlen(new_author) > 0 ? new_author : NULL);

    if (result == CODE_SUCCESS) {
        printf("Cap nhat thong tin sach thanh cong!\n");
    } else if (result == CODE_ERROR_INVALID_INPUT) {
        printf("Loi: Du lieu nhap vao qua dai.\n");
    }
    else {
        printf("Loi: Khong the cap nhat thong tin sach (Ma loi: %d).\n", result);
    }
}

void handle_delete_book (Library *lib) {

    printf("\n--- Xoa Sach ---\n");
    int book_id = get_int_input("Nhap ID sach can xoa: ");

    if (book_id == -999) return;

    int result = delete_book_from_library(lib, book_id);
    if (result == CODE_SUCCESS) {
        printf("Xoa sach thanh cong!\n");
    } else if (result == CODE_ERROR_NOT_FOUND) {
        printf("Loi: Khong tim thay sach voi ID %d.\n", book_id);
    } else if (result == CODE_ERROR_BOOK_BORROWED) {
        printf("Loi: Sach dang duoc muon, khong the xoa.\n");
    } else {
        printf("Loi: Khong the xoa sach (Ma loi: %d).\n", result);
    }
}

void handle_search_books_by_title (Library *library) {
    char query[BOOK_TITLE_LENGTH];

    printf("\n--- Tim Kiem Sach Theo Tieu De ---\n");

    if (get_string_input("Nhap tieu de can tim: ", query, sizeof(query)) != 0) {
         printf("Loi: Tieu de tim kiem khong duoc de trong.\n"); return;
    }
    /*
    * Mang luu tru cac sach tim thay
    */
    BookManagement* found_books[MAX_BOOKS]; 
    
    int count_found = 0;

    find_book_by_title_substring(library, query, found_books, MAX_BOOKS, &count_found);

    if (count_found > 0) {
        printf("Tim thay %d sach:\n", count_found);
        for (int i = 0; i < count_found; ++i) {
            display_book(found_books[i]);
        }
    } else {
        printf("Khong tim thay sach nao phu hop voi tieu de '%s'.\n", query);
    }
}

void handle_search_books_by_author (Library *lib) {

    char query[AUTHOR_NAME_LENGTH];
    printf("\n--- Tim Kiem Sach Theo Tac Gia ---\n");
     if (get_string_input("Nhap ten tac gia can tim: ", query, sizeof(query)) != 0) {
         printf("Loi: Ten tac gia tim kiem khong duoc de trong.\n"); return;
    }

    BookManagement* found_books[MAX_BOOKS];
    int count_found = 0;
    find_book_by_author_substring(lib, query, found_books, MAX_BOOKS, &count_found);

    if (count_found > 0) {
        printf("Tim thay %d sach cua tac gia '%s':\n", count_found, query);
        for (int i = 0; i < count_found; ++i) {
            display_book(found_books[i]);
        }
    } else {
        printf("Khong tim thay sach nao cua tac gia '%s'.\n", query);
    }
}


void handle_display_available_books(const Library *lib) {
    display_available_books(lib);
}


void handle_add_user (Library *library) {
    
    char name[USER_NAME_LENGTH];
    printf("\n--- Them Nguoi Dung Moi ---\n");
    if (get_string_input("Nhap ten nguoi dung: ", name, sizeof(name)) != 0) {
         printf("Loi: Ten nguoi dung khong duoc de trong.\n"); return;
    }

    int result = add_user_to_library(library, name);
    if (result == CODE_SUCCESS) {
        printf("Them nguoi dung thanh cong!\n");
    } else if (result == CODE_ERROR_CAPACITY_FULL) {
        printf("Loi: So luong nguoi dung da dat toi da.\n");
    } else if (result == CODE_ERROR_INVALID_INPUT) {
        printf("Loi: Ten nguoi dung khong hop le.\n");
    }
    else {
        printf("Loi: Khong the them nguoi dung (Ma loi: %d).\n", result);
    }
}

void handle_edit_user (Library *lib) {
    printf("\n--- Chinh Sua Thong Tin Nguoi Dung ---\n");
    int user_id = get_int_input("Nhap ID nguoi dung can chinh sua: ");
    if (user_id == -999) return;

    UserManagement* user = find_user_by_id(lib, user_id);
    if (!user) {
        printf("Loi: Khong tim thay nguoi dung voi ID %d.\n", user_id);
        return;
    }
    printf("Thong tin nguoi dung hien tai: %s (ID: %d)\n", user->name, user->id);

    char new_name[USER_NAME_LENGTH];
    if (get_string_input("Nhap ten moi (de trong neu khong thay doi): ", new_name, sizeof(new_name)) != 0 && strlen(new_name) == 0) {
        
        printf("Khong co thay doi nao duoc thuc hien.\n");
        return;
    }
    if (strlen(new_name) == 0) {
        printf("Khong co thay doi nao duoc thuc hien.\n");
        return;
    }


    int result = edit_user_in_library(lib, user_id, new_name);
    if (result == CODE_SUCCESS) {
        printf("Cap nhat thong tin nguoi dung thanh cong!\n");
    } else if (result == CODE_ERROR_INVALID_INPUT) {
         printf("Loi: Ten moi khong hop le hoac qua dai.\n");
    }
    else {
        printf("Loi: Khong the cap nhat thong tin nguoi dung (Ma loi: %d).\n", result);
    }
}

void handle_delete_user (Library *lib) {
    printf("\n--- Xoa Nguoi Dung ---\n");
    int user_id = get_int_input("Nhap ID nguoi dung can xoa: ");
    if (user_id == -999) return;

    int result = delete_user_from_library(lib, user_id);
    if (result == CODE_SUCCESS) {
        printf("Xoa nguoi dung thanh cong!\n");
    } else if (result == CODE_ERROR_NOT_FOUND) {
        printf("Loi: Khong tim thay nguoi dung voi ID %d.\n", user_id);
    } else if (result == CODE_ERROR_USER_MAX_BOOKS_REACHED) {
        printf("Loi: Nguoi dung dang muon sach, khong the xoa.\n");
    } else {
        printf("Loi: Khong the xoa nguoi dung (Ma loi: %d).\n", result);
    }
}


void handle_display_all_users(const Library *lib) {
    display_all_users_and_borrowed_books(lib);
}

void handle_borrow_book(Library *lib) {
    printf("\n--- Muon Sach ---\n");
    int user_id = get_int_input("Nhap ID nguoi dung muon sach: ");
    if (user_id == -999) return;
    int book_id = get_int_input("Nhap ID sach can muon: ");
    if (book_id == -999) return;

    int result = borrow_book_from_library(lib, user_id, book_id);
    if (result == CODE_SUCCESS) {
        printf("Muon sach thanh cong!\n");
    } else {
        printf("Loi muon sach: ");
        switch (result) {
            case CODE_ERROR_NOT_FOUND:
                printf("Nguoi dung hoac sach khong ton tai.\n");
                break;
            case CODE_ERROR_BOOK_NOT_AVAILABLE:
                printf("Sach khong co san hoac da duoc muon.\n");
                break;
            case CODE_ERROR_USER_MAX_BOOKS_REACHED:
                printf("Nguoi dung da muon toi da so luong sach cho phep.\n");
                break;
            default:
                printf("Loi khong xac dinh (Ma loi: %d).\n", result);
        }
    }
}

void handle_return_book(Library *lib) {
    printf("\n--- Tra Sach ---\n");
    int user_id = get_int_input("Nhap ID nguoi dung tra sach: ");
    if (user_id == -999) return;
    int book_id = get_int_input("Nhap ID sach can tra: ");
    if (book_id == -999) return;

    int result = return_book_to_library(lib, user_id, book_id);
    if (result == CODE_SUCCESS) {
        printf("Tra sach thanh cong!\n");
    } else {
        printf("Loi tra sach: ");
        switch (result) {
            case CODE_ERROR_NOT_FOUND:
                printf("Nguoi dung hoac sach khong ton tai.\n");
                break;
            case CODE_ERROR_BOOK_NOT_AVAILABLE: 
                printf("Sach khong trong trang thai da muon, hoac sai thong tin.\n");
                break;
            case CODE_ERROR_BOOK_NOT_BORROWED_BY_USER:
                printf("Nguoi dung nay khong muon cuon sach nay.\n");
                break;
            default:
                printf("Loi khong xac dinh (Ma loi: %d).\n", result);
        }
    }
}