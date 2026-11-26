#ifndef MEMBER_H
#define MEMBER_H
#include "lib_system.h"
#include <stdbool.h>

void loop_until_valid_num(char *number);
void validate_email(char *email);
member_t* find_member(int* id, member_t **member_db);
void validate_memberId(int *id, member_t** member_db);
void add_member(member_t** member_db);
void update_member_property(member_t* member, int *option);
void update_member(member_t **member_db);
void delete_member(member_t **member_db);
void print_header_member();
void print_separator();
void print_member(member_t *member);
void get_members(member_t **member_db);
void dueDate(char* date);
char* store_book_info(member_t *member, book_t *book);
bool has_book_copy(unsigned int id, member_t* member);
void borrow_book(book_t **book_db, member_t **member_db);
int book_borrowed(unsigned int bookID, member_t* member);
void update_member_borrow_history(int *idx, member_t* member);
void update_book_borrow_history(int* userID,book_t* book, member_t** member_db);
void return_book(book_t **book_db, member_t **member_db);

#endif