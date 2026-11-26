#ifndef PERSIST_H
#define PERSIST_H
#define BOOK_MAGIC   0x314B4F42u
#define MEM_MAGIC    0x314D454Du 
#define BOOKS_FILE   "books.dat"
#define MEMBERS_FILE "members.dat"

#include "lib_system.h"

int save_books(book_t **book_db);
int save_members(member_t **member_db);
int load_books(book_t **book_db);
int load_members(member_t **member_db);
void free_all(book_t **book_db, member_t **member_db);

#endif