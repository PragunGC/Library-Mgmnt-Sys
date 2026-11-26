#ifndef BOOK_H
#define BOOK_H
#include "lib_system.h"

book_t* find_book(int* id, book_t **book_db);
void validate_bookID(int *id, book_t** book_db);
void add_book(book_t** book_db);
void update_book_property(book_t* book, int *option);
void update_book(book_t **book_db);
void delete_book(book_t **book_db);
void get_books(book_t **book_db);

#endif