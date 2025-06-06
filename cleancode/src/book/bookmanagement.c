#include <stdio.h>
#include <string.h>
#include "bookmanagement.h"

void init_book (BookManagement *book, int id, const char *title, const char *author) {
    if (book == NULL || title == NULL || author == NULL) {
        return;
    }
    book->id = id;

    strncpy(book->title, title, BOOK_TITLE_LENGTH - 1);
    book->title[BOOK_TITLE_LENGTH - 1] = '\0';
    
    strncpy(book->author, author, AUTHOR_NAME_LENGTH - 1);
    book->author[AUTHOR_NAME_LENGTH - 1] = '\0';
    
    book->status = BOOK_STATUS_AVAILABLE;
}

void display_book (const BookManagement *book) {
    if (book == NULL) {
        printf("Error: Can't find book. \n");
        return;
    }
    printf("++++++++++++++++++++++++++++\n");
    printf("ID     : %d\n", book->id);
    printf("Title  : %s\n", book->title);
    printf("Author : %s\n", book->author);
    printf("Status : %s\n", (book->status == BOOK_STATUS_AVAILABLE) ? "Available": "Unvailable");
}

