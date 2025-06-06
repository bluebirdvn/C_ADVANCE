#ifndef _BOOKMANAGEMENT_H
#define _BOOKMANAGEMENT_H

#include "libdefines.h"

typedef struct{
	int id;
	char title[BOOK_TITLE_LENGTH];
	char author[AUTHOR_NAME_LENGTH];
	BookStatus status;
}BookManagement;

void init_book(BookManagement *book, int id, const char *title, const char *author);

void display_book(const BookManagement *book);

#endif 
