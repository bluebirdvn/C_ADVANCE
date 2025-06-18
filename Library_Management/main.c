#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "libdefines.h"
#include "bookmanagement.h"
#include "usermanagement.h"
#include "library.h"

#include "actionhandler.h"




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