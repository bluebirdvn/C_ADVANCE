#include <stdio.h>
#include <string.h>
#include "usermanagement.h"

void init_user(UserManagement *user, int id, const char *name){
    if(user == NULL || name == NULL){
        return;
    }

    user->id = id;
    strncpy(user->name, name, USER_NAME_LENGTH - 1);
    user->name[USER_NAME_LENGTH - 1] = '\0';
    user->borrowed_count = 0; 
}

void display_user(const UserManagement *user){
    if(user == NULL){
        printf("Not found infomation about the user");
        return;
    }
    printf("++++++++++++++++++++++");
    printf("ID: %d\n", user->id);
    printf("Name: %s\n", user->name);
    printf("Quantity book borrowed: %d\n", user->borrowed_count);

    if(user->borrowed_count > 0){
        printf("List book borrowed: \n");
        for(int i=0; i<user->borrowed_count; i++){
            printf("%d ", user->borrowed_book_ids[i]);
        }
        printf("\n");
    }
    printf("++++++++++++++++++++++");
}

