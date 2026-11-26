#ifndef LIB_SYSTEM_H
#define LIB_SYSTEM_H

//Constants for app
#define DB_SIZE 50
#define string_length 64
#define add_length 128
#define edition_length 16
#define date_length 11
#define phone_length 11
#define due_date 14
#define print_length 16
#define COLOR_BOLD "\033[1m"
#define COLOR_OFF "\033[m"

typedef struct Book{
    unsigned int bookID;
    char title[string_length];
    char author[string_length];
    char publications[string_length];
    char edition[edition_length];
    unsigned int stock;
    unsigned int availablity;
    unsigned int* borrowers;
    char closest_due_date[date_length];
    struct Book* next;
}book_t;

typedef struct User{
    unsigned int memberID;
    char name[string_length];
    char address[add_length];
    char phoneNo[phone_length];
    char email[string_length];
    unsigned int* books_borrowed[5]; 
    char* dueDates[5];
    struct User* next;
}member_t;

void clear_input_buffer(void);
void loop_until_string_not_empty(char *input, char* field, int buffer_size);
void loop_until_positive_number(int *num, char*field);
void remove_next_line(char* string);
int hash(int id);
void check_due_date(book_t **book_bd);
void print_string(char* str);
int main(int argc, char *argv[]);

#endif