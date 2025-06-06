#ifndef _USERMANAGEMENT_H
#define _USERMANAGEMENT_H
#include "libdefines.h"

typedef struct {
	int id;
	char name[USER_NAME_LENGTH];
	int borrowed_book_ids[MAX_BOOK_BORROW];
	int borrowed_count;
} UserManagement;

void init_user(UserManagement *user, int id, const char *name);

void display_user(const UserManagement *user);

#endif
