#ifndef _LIBRARY_H
#define _LIBRARY_H
#include "bookmanagement.h"
#include "usermanagement.h"
#include "libdefines.h"

typedef struct {
	BookManagement books[MAX_BOOKS];
	int book_count;
	int nex_book_id;

	UserManagement users[MAX_USERS];
	int user_count;
	int next_user_id;
}Library;


void init_library(Library *library);

int add_book_to_library(Library *library, const char *title, const char *author);

int add_user_to_library(Library *library, const char *name);

BookManagement* find_book_by_id(Library *library, int book_id);

void display_available_books(const Library *library);

UserManagement* find_user_by_id(Library *library, int user_id);

void display_all_users_and_borrowed_books(const Library *library);

int borrow_book_from_library(Library *library, int user_id, int book_id);

int return_book_to_library(Library *library, int user_id, int book_id);

int edit_book_in_library(Library *library, int book_id, const char *new_title, const char *new_author);

int delete_book_from_library(Library *library, int book_id);

void find_book_by_title_substring(const Library *library, const char *query_title, BookManagement* foundbook[], int max_results, int *count_found);

void find_book_by_author_substring(const Library *library, const char *query_author, BookManagement* found_books[], int max_results, int *count_found);

int edit_user_in_library(Library *library, int user_id, const char *new_name);

int delete_user_from_library(Library *library, int user_id);

#endif
