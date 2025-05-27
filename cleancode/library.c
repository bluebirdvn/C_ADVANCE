#include <stdio.h>
#include "library.h"
#include <string.h>


void init_library(Library *library){
    if(library == NULL){
        return;
    }
    library->book_count = 0;
    library->user_count = 0;
    library->nex_book_id = 2000;
    library->next_user_id = 1000;

}

int add_book_to_library(Library *library, const char *title, const char *author){
    if(library == NULL){
        return CODE_ERROR_GENERIC;
    }
    if( title == NULL || author == NULL || strlen(title) == 0 || strlen(author) == 0){
        return CODE_ERROR_INVALID_INPUT;
    }
    if(library->book_count >= MAX_BOOKS){
        return CODE_ERROR_CAPACITY_FULL;
    }
    BookManagement *new_book = &library->books[library->book_count];
    init_book(new_book, library->nex_book_id, title, author);
    library->book_count++;
    library->nex_book_id++;
    return CODE_SUCCESS;
}



int add_user_to_library(Library *library, const char *name){
    if(library == NULL){
        return CODE_ERROR_GENERIC;       
    }
    if(name == NULL || strlen(name) == 0){
        return CODE_ERROR_INVALID_INPUT;
    }
    if ( library->user_count > MAX_USERS ){
        return CODE_ERROR_CAPACITY_FULL;
    }
    
    UserManagement *new_user = &library->users[library->user_count];
    init_user(new_user, library->next_user_id, name);

    library->user_count++;
    library->next_user_id++;

    return CODE_SUCCESS;
}



BookManagement* find_book_by_id(Library *library, int book_id){
    if(library == NULL){
        return NULL;
    }
    for(int i=0; i < library->book_count; i++){
        if( library->books[i].id == book_id ){
            return &library->books[i];
        }
    }
    return NULL;
}

void display_available_books (const Library *library) {

    if (library == NULL){
        printf("Error: library infomation is not valid");
        return;
    }
    printf("\n---- List available book in library ----\n");
    int available_book = 0;
    if (library->book_count == 0) {
        printf("Library has not yet book");
        return;
    }

    for (int i = 0; i < library->book_count; ++i) {
        if (library->books[i].status == BOOK_STATUS_AVAILABLE) {
            display_book(&library->books[i]);
            available_book++;
        }
    } 

    if (available_book == 0) {
        printf("Hien tai khong co sach nao de muon.\n");
    }

    printf("-----------------------------------------\n");
}

UserManagement* find_user_by_id(Library *library, int user_id){
    if (library == NULL) {
        printf("Error: library infomation is not valid");
        return NULL;
    }

    for (int i  = 0; i < library->user_count; ++i) {
        if (library->users[i].id == user_id) {
            return &library->users[i];
        }
    }
    return NULL;
    
}

void display_all_users_and_borrowed_books(const Library *library){
    if (library == NULL) {
        printf("Error: library infomation is not valid");
        return;
    }

    printf("\n----List users and borrowed books----\n");
    if (library->user_count == 0) {
        printf("System has not yet user");
        return;
    }

    for (int i = 0; i < library->user_count; ++i){
        const UserManagement *user = &library->users[i];
        printf("====================================\n");
        printf("ID Nguoi dung: %d\n", user->id);
        printf("Ten           : %s\n", user->name);
        printf("So sach da muon: %d\n", user->borrowed_count);

        if (user->borrowed_count > 0) {
            printf("Chi tiet sach da muon:\n");
            for (int j = 0; j < user->borrowed_count; ++j) {
                int borrowed_book_id = user->borrowed_book_ids[j];
                BookManagement *borrowed_book = find_book_by_id((Library*)library, borrowed_book_id);
                if (borrowed_book) {
                    printf("  - ID: %d, Tieu de: %s\n", borrowed_book->id, borrowed_book->title);
                } else {
                    printf("  - ID: %d (Loi: Khong tim thay thong tin sach nay)\n", borrowed_book_id);
                }
            }
        }
        printf("=========================================\n");
    }
    printf("--------------------------------------------\n");
}


int borrow_book_from_library(Library *library, int user_id, int book_id){
    if (library == NULL) {
        return CODE_ERROR_GENERIC;
    }

    UserManagement *user = find_user_by_id(library, user_id);

    if (user == NULL) {
        return CODE_ERROR_NOT_FOUND;
    }

    BookManagement *book = find_book_by_id(library, book_id);

    if (book == NULL) {
        return CODE_ERROR_NOT_FOUND;
    }

    if (book->status == BOOK_STATUS_BORROWED) {
        return CODE_ERROR_BOOK_NOT_AVAILABLE;

    }

    if (user->borrowed_count > MAX_BOOKS) {
        return CODE_ERROR_USER_MAX_BOOKS_REACHED;
    }

    book->status = BOOK_STATUS_BORROWED;
    user->borrowed_book_ids[user->borrowed_count] = book_id;
    user->borrowed_count++;

    return CODE_SUCCESS;

}

int return_book_to_library(Library *library, int user_id, int book_id){
    if (library == NULL) {
        return CODE_ERROR_GENERIC;
    }

    UserManagement *user = find_user_by_id(library, user_id);

    if (user == NULL) {
        return CODE_ERROR_NOT_FOUND;
    }

    BookManagement *book = find_book_by_id(library, book_id);

    if (book == NULL) {
        return CODE_ERROR_NOT_FOUND;
    }

    if (book->status == BOOK_STATUS_AVAILABLE) {
        return CODE_ERROR_BOOK_NOT_AVAILABLE;
    }
    int found_borrowed_index = -1;

    for (int i = 0; i < user->borrowed_count; ++i) {
        if (user->borrowed_book_ids[i] == book_id) {
            found_borrowed_index = i;
            break;
        }
    }
    
    if (found_borrowed_index == -1) {
        return CODE_ERROR_BOOK_NOT_BORROWED_BY_USER;
    }

    book->status = BOOK_STATUS_AVAILABLE;

    for (int i = found_borrowed_index; i < user->borrowed_count - 1; ++i){
        user->borrowed_book_ids[i] = user->borrowed_book_ids[i+1];

    }

    user->borrowed_count--;
    return CODE_SUCCESS;
}

int edit_book_in_library(Library *library, int book_id, const char *new_title, const char *new_author) {
    if (library == NULL) {
        return CODE_ERROR_GENERIC;
    }

    BookManagement *book_to_edit = find_book_by_id(library, book_id);
    if (book_to_edit == NULL) {
        return CODE_ERROR_NOT_FOUND;
    }

    int changed = 0;
    if (new_title != NULL && strlen(new_title) > 0) {
        if (strlen(new_title) >= BOOK_TITLE_LENGTH) return CODE_ERROR_INVALID_INPUT;
        strncpy(book_to_edit->title, new_title, BOOK_TITLE_LENGTH - 1);
        book_to_edit->title[BOOK_TITLE_LENGTH - 1] = '\0';
        changed = 1;
    }

    if (new_author != NULL && strlen(new_author) > 0) {
        if (strlen(new_author) >= AUTHOR_NAME_LENGTH) return CODE_ERROR_INVALID_INPUT;
        book_to_edit->author[AUTHOR_NAME_LENGTH - 1] = '\0';
        changed = 1;
    }
    
    return changed ? CODE_SUCCESS : CODE_ERROR_GENERIC;
}

int delete_book_from_library(Library *library, int book_id){
    if (library == NULL) {
        return CODE_ERROR_GENERIC;
    }

    int book_index = -1;
    for (int i = 0; i < library->book_count; ++i) {
        if (library->books[i].id == book_id) {
            book_index = i;
            break;
        }
    }

    if (book_id == -1) {
        return CODE_ERROR_NOT_FOUND;
    }

    if (library->books[book_index].status == BOOK_STATUS_BORROWED) {
        return CODE_ERROR_BOOK_BORROWED;
    }

    for (int i = book_index; i < library->book_count - 1; ++i) {
        library->books[i] = library->books[i+1];
    }

    library->book_count--;

    return CODE_SUCCESS;
}

void find_book_by_title_substring(const Library *library, const char *query_title, BookManagement* found_books[], int max_results, int *count_found) {
    if (library == NULL || query_title == NULL || found_books == NULL || count_found == NULL) {
        if (count_found) *count_found = 0;
        return;
    }

    *count_found = 0;
    if (strlen(query_title) == 0) return; 

    for (int i = 0; i < library->book_count; ++i) {
        
        if (strstr(library->books[i].title, query_title) != NULL) {
            if (*count_found < max_results) {
                
                found_books[*count_found] = (BookManagement*)&library->books[i];
                (*count_found)++;
            } else {
                break;
            }
        }
    }
}

void find_book_by_author_substring(const Library *library, const char *query_author, BookManagement* found_books[], int max_results, int *count_found) {
    if (library == NULL || query_author == NULL || found_books == NULL || count_found == NULL) {
        if (count_found) *count_found = 0;
        return;
    }

    *count_found = 0;
    if (strlen(query_author) == 0) return;

    for (int i = 0; i < library->book_count; ++i) {
        if (strstr(library->books[i].author, query_author) != NULL) {
            if (*count_found < max_results) {
                found_books[*count_found] = (BookManagement*)&library->books[i];
                (*count_found)++;
            } else {
                break;
            }
        }
    }
}




int edit_user_in_library(Library *library, int user_id, const char *new_name) {
    if (library == NULL) {
        return CODE_ERROR_GENERIC;
    }

    UserManagement *user_to_edit = find_user_by_id(library, user_id);
    if (user_to_edit == NULL) {
        return CODE_ERROR_NOT_FOUND;
    }

    if (new_name != NULL && strlen(new_name) > 0) {
        if (strlen(new_name) >= USER_NAME_LENGTH) return CODE_ERROR_INVALID_INPUT; // Quá dài
        strncpy(user_to_edit->name, new_name, USER_NAME_LENGTH - 1);
        user_to_edit->name[USER_NAME_LENGTH - 1] = '\0';
        return CODE_SUCCESS;
    }
    return CODE_ERROR_INVALID_INPUT;
}

int delete_user_from_library(Library *library, int user_id) {
    if (library == NULL) {
        return CODE_ERROR_GENERIC;
    }

    int user_index = -1;
    for (int i = 0; i < library->user_count; ++i) {
        if (library->users[i].id == user_id) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        return CODE_ERROR_NOT_FOUND; 
    }

    if (library->users[user_index].borrowed_count > 0) {
        return CODE_ERROR_USER_MAX_BOOKS_REACHED;
    }

    for (int i = user_index; i < library->user_count - 1; ++i) {
        library->users[i] = library->users[i + 1];
    }

    library->user_count--;
    return CODE_SUCCESS;
}




