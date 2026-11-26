#include "lib_system.h"
#include "book.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<stdbool.h>

/**
 * Finds a book in the book db
 * Returns address of book if book found else NULL
 */
book_t* find_book(int* id, book_t **book_db){
    int* hashed_id = malloc(sizeof(int));
    *hashed_id = hash(*id);
    bool found = false;
    book_t* book = NULL;
    for(book = book_db[*hashed_id];book;book=book->next){
        if(book->bookID == *id){
            found = true;
            break;
        }
    }
    free(hashed_id);
    if(found)
        return book;
    else
        return NULL;
}

/**
 * Validates if the memeberID already exists
 */
void validate_bookID(int *id, book_t** book_db){
    book_t* book = find_book(id, book_db);
    if(book!=NULL){
         printf("\nBookID already taken! Enter ID again: ");
            scanf("%u",id);
            clear_input_buffer();
            validate_bookID(id,book_db);
    }
}

/**
 * Adds a new book to db
 */
void add_book(book_t** book_db){
    book_t* newbook = calloc(1,sizeof(book_t));
    printf("\nEnter id for new book : ");
    scanf("%u",&newbook->bookID);
    loop_until_positive_number(&newbook->bookID,"BookID");
    validate_bookID(&newbook->bookID,book_db);

    printf("\nEnter title of the book: ");
    fgets(newbook->title, sizeof(newbook->title), stdin);
    loop_until_string_not_empty(newbook->title, "Book Title", string_length);
    /**
    *The new line from input is effecting the printing
    *strcspn finds the position new line char 
    *and in that poition replace with end of string
    */
    remove_next_line(newbook->title);
    
    printf("\nEnter author of the book: ");
    fgets(newbook->author, sizeof(newbook->author), stdin);
    loop_until_string_not_empty(newbook->author, "Book author", string_length);
    remove_next_line(newbook->author);
    
    printf("\nEnter publications of the book: ");
    fgets(newbook->publications,sizeof(newbook->publications),stdin);
    loop_until_string_not_empty(newbook->publications, "Book publication house", string_length);
    remove_next_line(newbook->publications);
    
    printf("\nEnter edition of the book: ");
    fgets(newbook->edition,sizeof(newbook->edition),stdin);
    loop_until_string_not_empty(newbook->edition, "Book edition", edition_length);
    remove_next_line(newbook->edition);
    
    printf("\nEnter total number of books being added into library: ");
    scanf("%u",&newbook->stock);
    loop_until_positive_number(&newbook->stock,"Book stock");
    newbook->availablity = newbook->stock;
    newbook->next = NULL;
    
    int idx = hash(newbook->bookID);
    //Insert at head
    newbook->next = book_db[idx];
    book_db[idx] = newbook;
    printf("New book added!");
}

/**
 * Updates a specific property of a book
 */
void update_book_property(book_t* book, int *option){
    switch(*option){
        case 1:
            printf("\nEnter new book title: ");
            char* new_book_title = malloc(string_length*sizeof(char));
            fgets(new_book_title,string_length,stdin);
            loop_until_string_not_empty(new_book_title, "Book Title", string_length);
            remove_next_line(new_book_title);
            strcpy(book->title,new_book_title);
            printf("\nSuccessfully updated book title!");
            free(new_book_title);
            break;
        case 2:
            printf("\nEnter new book author: ");
            char* new_book_author = malloc(string_length*sizeof(char));
            fgets(new_book_author,string_length,stdin);
            loop_until_string_not_empty(new_book_author, "Book author", string_length);
            remove_next_line(new_book_author);
            strcpy(book->author,new_book_author);
            printf("\nSuccessfully updated book author!");
            free(new_book_author);
            break;
        case 3:
            printf("\nEnter new book publication house: ");
            char* new_book_publication = malloc(string_length*sizeof(char));
            fgets(new_book_publication,string_length,stdin);
            loop_until_string_not_empty(new_book_publication, "Book publication house", string_length);
            remove_next_line(new_book_publication);
            strcpy(book->publications,new_book_publication);
            printf("\nSuccessfully updated book publication house!");
            free(new_book_publication);
            break;
        case 4:
            printf("\nEnter new book edition: ");
            char* new_book_edition = malloc(edition_length*sizeof(char));
            fgets(new_book_edition,edition_length,stdin);
            loop_until_string_not_empty(new_book_edition, "Book edition", edition_length);
            remove_next_line(new_book_edition);
            strcpy(book->edition,new_book_edition);
            printf("\nSuccessfully updated book edition!");
            free(new_book_edition);
            break;
        case 5:
            printf("\nEnter new book stock: ");
            unsigned int* new_stock = malloc(sizeof(unsigned int));
            scanf("%u",new_stock);
            loop_until_positive_number(new_stock,"Book stock");
            while(true){
                if((int)book->availablity + ((int)*new_stock - (int)book->stock)<0){
                    printf("\nInvalid stock value, please enter again: ");
                    scanf("%u", new_stock);
                }else{
                    break;
                }

            }
            book->availablity = book->availablity + (*new_stock - book->stock);
            //Resize borrowers array
            unsigned int *tmp = realloc(book->borrowers, *new_stock * sizeof *tmp);
            if(!tmp)
                break;
            book->borrowers = tmp;
            //Set new locations to 0;
            if (*new_stock > book->stock)
                memset(book->borrowers + book->stock, 0, (*new_stock - book->stock) * sizeof *book->borrowers);
            book->stock = *new_stock;
            printf("\nSuccessfully updated book stock!");
            free(new_stock);
            break;
    }
}

/**
 * Allow user to update multiple properties of a book, if exists
 */
void update_book(book_t **book_db){
    printf("\nID of book you want to update: ");
    unsigned int *update_id = malloc(sizeof(unsigned int));
    scanf("%u", update_id);
    clear_input_buffer();
    book_t *book = find_book(update_id,book_db);
    if(book == NULL){
        printf("Book to be updated not found; bookID %u, please try with another bookID.",*update_id);
        free(update_id);
        return;
    }
    printf("Book found! What would you like to update?\nFor multiple just join the options; Eg: If for Edition and stock input 45");
    char *choice = malloc(5*sizeof(char));
    printf("\n 1. Book title");
    printf("\n 2. Book author");
    printf("\n 3. Book publication house");
    printf("\n 4. Book edition");
    printf("\n 5. Book stock\nOption(s): ");
    fgets(choice,sizeof(choice),stdin);
    remove_next_line(choice);
    for(int i =0;i<strlen(choice);i++){
        if(choice[i] <'1' || choice[i]>'5'){
            char new_choice = '1';
            while(new_choice !='e'){
                printf("\nInvalid choice %c\nEnter a valid option or 'e' to end update ",choice[i]);
                char new_choice;
                scanf(" %c",&new_choice);
                if(new_choice >='1' || new_choice<='5'){
                    choice[i] = new_choice;
                    break;
                }
            }
        }
    }
    int *option = malloc(sizeof(int));
    for(int i =0;i<strlen(choice);i++){
        *option = choice[i] - '0';
        update_book_property(book,option);
    }
    printf("Book %d has been updated!",*update_id);
    free(option);
    free(choice);
    free(update_id);
}

/**
 * Deletes a book from the book db if exists
 */
void delete_book(book_t **book_db){
    printf("\nID of book you want to delete: ");
    unsigned int *delete_id = malloc(sizeof(int));
    scanf("%u", delete_id);
    clear_input_buffer();
    book_t *book = find_book(delete_id,book_db);
    if(book == NULL){
        printf("Book to be deleted not found; bookID %u, please try with another bookID.",*delete_id);
        free(delete_id);
        return;
    }
    int *delete_idx = malloc(sizeof(int));
    *delete_idx = hash(book->bookID);
    if(book_db[*delete_idx] == book){
        book_db[*delete_idx] = book->next;
        free(book);
    }
    else{
        book_t *ptr = book_db[*delete_idx];
        for(*ptr;ptr;ptr=ptr->next){
            if(ptr->next->bookID == *delete_id){
                ptr->next = book->next;
                free(book);
                break;
            }
        }
    }
    free(delete_id);
    free(delete_idx);
    printf("Book successfuly deleted!");
}

/**
 * Prints all books in the library
 */
void get_books(book_t **book_db){
    int *count = calloc(1,sizeof(int));
    for(int i = 0;i<DB_SIZE;i++){
        if(book_db[i]==NULL){
            *count = *count + 1;
            continue;
        }
        book_t *book = book_db[i];
        while(book){
            printf(COLOR_BOLD "BookID :" COLOR_OFF "%d | ",book->bookID);
            printf(COLOR_BOLD "Book Title:" COLOR_OFF "%s | ",book->title);
            printf(COLOR_BOLD "Book Author:" COLOR_OFF "%s | ",book->author);
            printf(COLOR_BOLD "Book Publication House:" COLOR_OFF "%s | ",book->publications);
            printf(COLOR_BOLD "Book Edition:" COLOR_OFF "%s | ",book->edition);
            printf(COLOR_BOLD "Book total stock:" COLOR_OFF "%d | ", book->stock);
            printf(COLOR_BOLD "Book total availablity:" COLOR_OFF "%d | ", book->availablity);
            if(book->stock != book->availablity){
                printf(COLOR_BOLD "Closest due date for book:" COLOR_OFF "%s | ", book->closest_due_date);
                printf(COLOR_BOLD "Member with closest due date:" COLOR_OFF "%u | ", book->borrowers[0]);
            }
            book = book->next;
            printf("\n");
        }
    }
    if(*count==DB_SIZE)
        printf("\nNo books in library!");
    free(count);
}