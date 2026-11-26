//Clearing input buffer after any sort of scanf to not cause any issues with fgets
#include "lib_system.h"
#include "book.h"
#include "member.h"
#include "persist.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<stdbool.h>
#include <time.h>

void clear_input_buffer(void) {
    int c;
    /**
     * This loop will increment the char in buffer until it reached new line 
     * so that it clears the input buffer from scanf to allow fgets to not skip
     * (The new line from scanf being considerd for the next fgets skipping that input)
     */
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard characters until newline or End-Of-File
    }
}

/**
 * Loop a string input untill it gets a non empty input
 * Validates that string input is not just next line
 */
void loop_until_string_not_empty(char *input, char* field, int buffer_size){
    while(strcmp(input,"\n")==0){
        printf("\n%s cannot be empty please enter again!: ",field);
        fgets(input,buffer_size,stdin);
    }
}

/**
 * Loop a int input untill it gets a positive int
 * Validates that int input isn't negative
 */
void loop_until_positive_number(int *num, char*field){
    while(*num < 0){
        printf("\n%s cannot be negative please enter again!: ",field);
        scanf("%u",num);
    }
    clear_input_buffer();
}

/**
*Removes the new line from end of string
The new line from input is effects the printing
*strcspn finds the position new line char 
*and in that poition replace with end of string
*/
void remove_next_line(char* string){
    //If string dosn't have new line it stays in buffer,
    //So clear buffer and do not remove new line
    if(!strchr(string, '\n')){
        clear_input_buffer();
        return;
    }
    string[strcspn(string, "\r\n")] = '\0';
}

/**
 * Checks if any book has a dueDate for today
 */
void check_due_date(book_t **book_bd){
    printf(COLOR_BOLD "Book due today!\n" COLOR_OFF);
    int count = 0;
    time_t now = time(NULL);
    struct tm t = *localtime(&now);
    char today[date_length];
    strftime(today,date_length,"%Y-%m-%d", &t);
    for(int i =0;i<DB_SIZE;i++){
        if(book_bd[i]==NULL)
            continue;
        book_t *head = book_bd[i];
        while(head){
            if(strcmp(head->closest_due_date,today)==0){
                count++;
                printf("Book %d borrowed by %d",head->bookID,head->borrowers[0]);
            }
            head = head->next;
        }
    }
    if(count==0){
        printf("No books due today!\n");
    }
}

//Simple hasing to hash lookup
int hash(int id){
    return id % DB_SIZE;
}

int main(int argc, char *argv[]){
    //Switch input stream to arg 1, which is the input file
    //Allow more args ,like a flag to retain or clear console
    if (argc > 1) {
        if (!freopen(argv[1], "r", stdin)){
            perror("could not open input file. Resuming normal operations");
            //If failed to open file, switch to keyboard
            if(!freopen("CON","r",stdin)){
                return 1;
            }
        }
    }
    printf("--------------------------Welcome To-----------------------\n");
    printf("   ____  ____ _     _     _                          \n");
    printf("  / ___|/ ___( )___| |   (_) |__  _ __ __ _ _ __ _   _ \n");
    printf(" | |  _| |   |// __| |   | | '_ \\| '__/ _` | '__| | | |\n");
    printf(" | |_| | |___  \\__ \\ |___| | |_) | | | (_| | |  | |_| |\n");
    printf("  \\____|\\____| |___/_____|_|_.__/|_|  \\__,_|_|   \\__, |\n");
    printf("                                                  |___/ \n");
    printf("-----------------------------------------------------------\n");
    printf("===========================================================");
    book_t **book_db = calloc(DB_SIZE, sizeof(book_t *)); // Array of pointers to struct
    member_t **member_db = calloc(DB_SIZE, sizeof(member_t *));
    int *load_res = calloc(1,sizeof(int));
    printf("\nLoading book data...!\n");
    *load_res = load_books(book_db);
    if(*load_res!=0){
        printf("There was some failure in book data!\n");
    }else{
        printf("Loaded book data!\n");
    }
    printf("Loading member data...!\n");
    *load_res = load_members(member_db);
    if(*load_res!=0){
        printf("There was some failure in member data!\n");
    }else{
        printf("Loaded member data!\n");
    }
    check_due_date(book_db);
    while(true){
        printf("What would you like to do?\n");
        printf("1. Add book to library\n");
        printf("2. Update existing book\n");
        printf("3. Delete book from library\n");
        printf("4. Get all books in library\n");
        printf("5. Register new member to library\n");
        printf("6. Update information of existing member of library\n");
        printf("7. Remove memeber from library\n");
        printf("8. Get all members of library\n");
        printf("9. Issue a book to member\n");
        printf("10. Return a book borrowed by a member\n");
        printf("e. To exit application\n");
        printf("Option: ");
        char line[64]; //Option can now be 2 digits(10)
        //Try to read from input stream
        if (!fgets(line, sizeof line, stdin)) {
            // input file ran out (EOF): switch stdin back to the keyboard
            if (!freopen("CON", "r", stdin))   // "CON" = Windows console
                return 1;                      // can't reach keyboard, quit
            clearerr(stdin);
            continue;                          // show the menu again
        }
        //Based on the input stream the fgets and scanf work
        //If input stream is from opened file, fgets and scanf take from the file, else wait from keyboard
        remove_next_line(line);
        // See if exit char is entered before converting to int
        if(strcmp(line,"e") == 0 || strcmp(line,"E")==0){
            printf("Saving book data...!\n");
            save_books(book_db);
            printf("Saved book data!\n");
            printf("Saving member data...!\n");
            save_members(member_db);
            printf("Saved member data!\n");
            printf("Thank You!\n");
            printf(".....Exiting!");
            free_all(book_db,member_db);
            free(book_db);
            free(member_db);
            break;
        }
        int choice = strtol(line,NULL,10); // Converts string to long
        switch (choice)
        {
        case 1:
            add_book(book_db);
            break;
        case 2:
            update_book(book_db);
            break;
        case 3:
            delete_book(book_db);
            break;
        case 4:
            get_books(book_db);
            break;
        case 5:
            add_member(member_db);
            break;
        case 6:
            update_member(member_db);
            break;
        case 7:
            delete_member(member_db);
            break;
        case 8:
            get_members(member_db);
            break;
        case 9:
            borrow_book(book_db,member_db);
            break;
        case 10:
            return_book(book_db,member_db);
            break;
        default:
            printf("\nInvalid option");
            break;
        }
    }
}