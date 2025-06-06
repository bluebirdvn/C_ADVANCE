#ifndef _ACTIONHANDLER_H
#define _ACTIONHANDLER_H
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "libdefines.h"
#include "bookmanagement.h"
#include "usermanagement.h"
#include "library.h"

void clear_input_buffer();
int get_string_input(const char *prompt, char *buffer, int buffer_size);
int get_int_input(const char *prompt);
void display_main_menu();
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

#endif