#include "lib_system.h"
#include "member.h"
#include "book.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/**
 * Validates phone number of a member
 */
void loop_until_valid_num(char *number){
    //Shouldn't consider newline as a char
    remove_next_line(number);
    if(strlen(number)!=10){
        printf("\nPhone number must have 10 digits! Enter number again: ");
        fgets(number,phone_length,stdin);
        loop_until_valid_num(number);
    }
    char* ptr = number;
    while(*ptr){
        if(*ptr < '0' || *ptr > '9'){
            printf("\nPhone number must have only digits between 0 to 9! Enter number again: ");
            fgets(number,phone_length,stdin);
            loop_until_valid_num(number);
        }
        ptr++;
    }                                                                                                                                                                                                     
}

/**
 * Validates email address of a member
 */
void validate_email(char *email){
    remove_next_line(email);
    if(strlen(email)==0){
        strcpy(email,"NULL");
        return;
    }
    if(strchr(email,'@') == NULL){
        printf("\nEmail invalid! Enter email address again: ");
        fgets(email,string_length,stdin);
        validate_email(email);
    }
}

/**
 * Finds a member in the member db
 * Returns address of member if member found else NULL
 */
member_t* find_member(int *id, member_t** member_db){
    int* hashed_id = malloc(sizeof(int));
    *hashed_id = hash(*id);
    bool found = false;
    member_t* member = NULL;
    for(member = member_db[*hashed_id];member;member=member->next){
        if(member->memberID== *id){
            found = true;
            break;
        }
    }
    free(hashed_id);
    if(found)
        return member;
    else
        return NULL;
}

/**
 * Validates if the memberID already exists
 */
void validate_memberId(int *id, member_t** member_db){
    member_t* member = find_member(id,member_db);
    if(member!=NULL){
        printf("\nMemberID already taken! Enter ID again: ");
        scanf("%u",id);
        clear_input_buffer();
        validate_memberId(id,member_db);
    }
}

/**
 * Adds a new member to db
 */
void add_member(member_t** member_db){
    member_t* newmember = calloc(1,sizeof(member_t));
    printf("\nEnter id for new member : ");
    scanf("%u",&newmember->memberID);
    loop_until_positive_number(&newmember->memberID,"UserID");
    validate_memberId(&newmember->memberID,member_db);

    printf("\nEnter name of the new member: ");
    fgets(newmember->name, sizeof(newmember->name), stdin);
    loop_until_string_not_empty(newmember->name, "Member name", string_length);
    /**
    *The new line from input is effecting the printing
    *strcspn finds the position new line char 
    *and in that poition replace with end of string
    */
    remove_next_line(newmember->name);
    
    printf("\nEnter address of member: ");
    fgets(newmember->address, sizeof(newmember->address), stdin);
    loop_until_string_not_empty(newmember->address, "Member address", string_length);
    remove_next_line(newmember->address);
    
    printf("\nEnter phone number of member: ");
    fgets(newmember->phoneNo,sizeof(newmember->phoneNo),stdin);
    loop_until_valid_num(newmember->phoneNo);
    
    printf("\nEnter email of member: ");
    fgets(newmember->email,sizeof(newmember->email),stdin);
    //Email not compulsary
    validate_email(newmember->email);
    
    int idx = hash(newmember->memberID);
    //Insert at head
    newmember->next = member_db[idx];
    member_db[idx] = newmember;
    printf("New member has been registered!");
}

/**
 * Prints the table header for when getting books
 */
void print_header_member(){
    printf(COLOR_BOLD "%-8s | %-*s | %-*s | %-*s | %-*s | %-26s \n" COLOR_OFF,
    "MemberID", print_length, "Name", print_length, "Address", phone_length,"Ph. No.", print_length,"Email", "Books borrowed and Due date");
}

/**
 * Prints the table seperator for when getting books
 */
void print_separator(){
    printf("%.*s-+-%.*s-+-%.*s-+-%.*s-+-%.*s-+-%.*s\n",
           8,            "----------------------------------------------------------------",
           print_length, "----------------------------------------------------------------",
           print_length, "----------------------------------------------------------------",
           phone_length, "----------------------------------------------------------------",
           print_length, "----------------------------------------------------------------",
           27,           "----------------------------------------------------------------");
}

/**
 * Prints a member row
 */
void print_member(member_t *member){
    int first = 1;

    //Check if member has borrowed books
    for (int i = 0; i < 5; i++) {
        if (member->books_borrowed[i] == NULL)
            break;

        char entry[64];
        snprintf(entry, sizeof(entry), "[%u - %s]", *member->books_borrowed[i],member->dueDates[i]);

        if (first) {
            printf("%-8u | ", member->memberID);
            print_string(member->name); 
            print_string(member->address);
            printf("%-*.*s | ", phone_length, phone_length, member->phoneNo);
            print_string(member->email);
            printf("%s\n", entry);
            first = 0;
        } else {
            //Blank cells so the entry lines up under the first one
            printf("%-8s | %-*s | %-*s | %-*s | %-*s | %s\n",
                   "",
                   print_length, "",
                   print_length, "",
                   phone_length, "",
                   print_length, "",
                   entry);
        }
    }

    //If no books, still print member data
    if (first) {
        printf("%-8u | ", member->memberID);
        print_string(member->name);
        print_string(member->address);
        printf("%-*.*s | ", phone_length, phone_length, member->phoneNo);
        print_string(member->email);
        printf("-\n");
    }
}

/**
 * Prints all members of the library
 */
void get_members(member_t** member_db){
    bool has_members = false;
    for(int i = 0;i<DB_SIZE;i++){
        if(member_db[i] != NULL){
            has_members = true;
            break;
        }
    }
    if(!has_members){
        printf("\nNo books in library!");
        return;
    }
    print_separator();
    print_header_member();
    print_separator();
    for(int i = 0;i<DB_SIZE;i++){
        member_t* member = member_db[i];
        while(member){
            print_member(member);
            member = member->next;
        }
    }
    print_separator();
}

/**
 * Deletes a memger from the member_db if exists
 */
void delete_member(member_t **member_db){
    printf("\nID of member you want to delete: ");
    unsigned int *delete_id = malloc(sizeof(int));
    scanf("%u", delete_id);
    clear_input_buffer();
    member_t *member = find_member(delete_id,member_db);
    if(member == NULL){
        printf("Member to be deleted not found; memberID %u, please try with another memberID.",*delete_id);
        free(delete_id);
        return;
    }
    int *delete_idx = malloc(sizeof(int));
    *delete_idx = hash(member->memberID);
    if(member_db[*delete_idx] == member){
        member_db[*delete_idx] = member->next;
        free(member);
    }
    else{
        member_t *ptr = member_db[*delete_idx];
        for(ptr;ptr;ptr=ptr->next){
            if(ptr->next->memberID == *delete_id){
                ptr->next = member->next;
                free(member);
                break;
            }
        }
    }
    free(delete_id);
    free(delete_idx);
    printf("Member successfuly deleted!");
}

/**
 * Updates a specific property of a member
 */
void update_member_property(member_t* member, int *option){
    switch(*option){
        case 1:
            printf("\nEnter new member name: ");
            char* new_member_name = malloc(string_length*sizeof(char));
            fgets(new_member_name,string_length,stdin);
            loop_until_string_not_empty(new_member_name, "Member name", string_length);
            remove_next_line(new_member_name);
            strcpy(member->name,new_member_name);
            printf("\nSuccessfully updated member name!");
            free(new_member_name);
            break;
        case 2:
            printf("\nEnter new member address: ");
            char* new_member_address = malloc(string_length*sizeof(char));
            fgets(new_member_address,string_length,stdin);
            loop_until_string_not_empty(new_member_address, "Member address", string_length);
            remove_next_line(new_member_address);
            strcpy(member->address,new_member_address);
            printf("\nSuccessfully updated member address!");
            free(new_member_address);
            break;
        case 3:
            printf("\nEnter new member phone no: ");
            char* new_member_phone_no = malloc(phone_length*sizeof(char));
            fgets(new_member_phone_no,phone_length,stdin);
            loop_until_valid_num(new_member_phone_no);
            strcpy(member->phoneNo,new_member_phone_no);
            printf("\nSuccessfully updated member phone number!");
            free(new_member_phone_no);
            break;
        case 4:
            printf("\nEnter new member email address: ");
            char* new_member_email = malloc(string_length*sizeof(char));
            fgets(new_member_email, string_length,stdin);
            validate_email(new_member_email);
            strcpy(member->email,new_member_email);
            printf("\nSuccessfully updated member email address!");
            free(new_member_email);
            break;
        
    }
}

/**
 * Updates member information if member exists in db
 */
void update_member(member_t **member_db){
    printf("\nID of member you want to update: ");
    unsigned int *update_id = malloc(sizeof(int));
    scanf("%u", update_id);
    clear_input_buffer();
    member_t *member = find_member(update_id,member_db);
    if(member == NULL){
        printf("Member to be updated not found; memberID %u, please try with another memberID.",*update_id);
        free(update_id);
        return;
    }
    printf("Member found! What would you like to update?\nFor multiple just join the options; Eg: If for Name and phone input 13");
    char *choice = malloc(4*sizeof(char));
    printf("\n 1. Member name");
    printf("\n 2. Member address");
    printf("\n 3. Member phone number");
    printf("\n 4. Member email address\nOption(s): ");
    fgets(choice,sizeof(choice),stdin);
    remove_next_line(choice);
    for(int i =0;i<strlen(choice);i++){
        if(choice[i] <'1' || choice[i]>'4'){
            char new_choice = '1';
            while(new_choice !='e'){
                printf("\nInvalid choice %c\nEnter a valid option or 'e' to end update ",choice[i]);
                char new_choice;
                scanf(" %c",&new_choice);
                if(new_choice >='1' || new_choice<='4'){
                    choice[i] = new_choice;
                    break;
                }
            }
        }
    }
    int *option = malloc(sizeof(int));
    for(int i =0;i<strlen(choice);i++){
        *option = choice[i] - '0';
        update_member_property(member,option);
    }
    printf("Member %d has been updated!",*update_id);
    free(option);
    free(choice);
    free(update_id);
}

/**
 * Calculates due date
 */
void dueDate(char* date){
    time_t now = time(NULL);
    struct tm t = *localtime(&now);
    t.tm_mday += due_date;
    t.tm_hour  = 12;        // noon avoids DST edge cases
    t.tm_isdst = -1;        // let mktime work out DST itself
    mktime(&t);
    strftime(date,date_length,"%Y-%m-%d", &t);
}

/**
 * Store required information in the member and book entity
 */
char* store_book_info(member_t *member, book_t *book){
    for(int i=0;i<5;i++){
        if(member->books_borrowed[i] == NULL){
            member->books_borrowed[i] = malloc(sizeof(unsigned int));
            member->dueDates[i] = malloc(date_length*sizeof(char));
            *member->books_borrowed[i] = book->bookID;
            dueDate(member->dueDates[i]);
            if(book->closest_due_date[0] == '\0'){
                strcpy(book->closest_due_date,member->dueDates[i]);
            }
            book->availablity = book->availablity - 1;
            if(book->borrowers == NULL){
                book->borrowers = calloc(book->stock,sizeof(unsigned int));
                book->borrowers[0] = member->memberID;
            }else{
                for(unsigned int j =0;j<book->stock;j++){
                    if(book->borrowers[j]==0){
                        book->borrowers[j]=member->memberID;
                        break;
                    }
                }
            }
            return member->dueDates[i];
        }
        continue;
    }
    return NULL;
}

/**
 * See if member already has a copy of the book 
 */
bool has_book_copy(unsigned int id, member_t* member){
    if(*member->books_borrowed == NULL)
        return false;
    for(int i=0;i<5;i++){
        if(*member->books_borrowed[i]==id){
            return true;
        }
    }
    return false;
}

/**
 * Allows a member to check out a book
 */
void borrow_book(book_t **book_db, member_t **member_db){
    unsigned int *borrow_id = malloc(sizeof(int));
    printf("\nID of book to be borrowed: ");
    scanf("%u", borrow_id);
    clear_input_buffer();
    book_t *book = find_book(borrow_id,book_db);
    if(book == NULL){
        printf("Book to be borrowed not found; bookID %u, please try with another bookID.",*borrow_id);
        free(borrow_id);
        return;
    } else if(book->availablity == 0){
        printf("Book to be borrowed isn't available; bookID %u, please try with another bookID.",*borrow_id);
        free(borrow_id);
        return;
    }
    printf("\nID of member to whom the book needs to be issues to: ");
    scanf("%u", borrow_id);
    clear_input_buffer();
    member_t *member = find_member(borrow_id,member_db);
    if(member == NULL){
        printf("Member borrowing the book not registered; memberID %u, please try with another memberID.",*borrow_id);
        free(borrow_id);
        return;
    } else if(member->books_borrowed[4] != NULL){
        printf("Member has already borrowed max number of books; Cannot issue another book until one if returned");
        free(borrow_id);
        return;
    }
    //Check if user has borrowed this book
    if(has_book_copy(book->bookID,member)){
        printf("Member has already borrowed this book %u! Cannot issue the same book again",book->bookID);
        free(borrow_id);
        return;
    }
    char* res = store_book_info(member,book);
    printf("A copy of %s has been issued to %s. Due date is %s",book->title,member->name,res);
    free(res);
    free(borrow_id);
}

/**
 * Find index of book in member entity
 */
int book_borrowed(unsigned int bookID, member_t* member){
    for(int i=0;i<5;i++){
        if(*member->books_borrowed[i] == bookID){
            return i;
        }
    }
    return -1;
}

/**
 * Updates the member entity book related properties
 */
void update_member_borrow_history(int *idx, member_t* member){
    for(int i = *idx;i<4;i++){
        member->books_borrowed[i] = member->books_borrowed[i+1];
        member->dueDates[i] = member->dueDates[i+1];
    }
    member->books_borrowed[4] = NULL;
    member->dueDates[4] = NULL;
}

/**
 * Updates the book entity borrow/return related properties
 */
void update_book_borrow_history(int* memberID,book_t* book, member_t** member_db){
    //Find the returning member's idx in book entity
    unsigned int idx = book->stock;
    for (unsigned int i = 0; i < book->stock; i++) {
        if (book->borrowers[i] == (unsigned int)*memberID){ 
            idx = i; 
            break; 
        }
    }

    //Shift values to left from removing position
    for (unsigned int j = idx; j< book->stock - 1; j++)
        book->borrowers[j] = book->borrowers[j + 1];
    book->borrowers[book->stock - 1] = 0; // Make last empty
    book->availablity++;

    //Update closest due date only if the first borrower is returning
    if (idx==0) {
        book->closest_due_date[0] = '\0';
        if (book->borrowers[0] != 0) {
            member_t *next = find_member(&book->borrowers[0], member_db);
            if (next) {
                for (int i = 0; i < 5; i++) {
                    if (next->books_borrowed[i] && next->dueDates[i] &&
                        *next->books_borrowed[i] == book->bookID) {
                        strcpy(book->closest_due_date, next->dueDates[i]);
                        break;
                    }
                }
            }
        }
    }
}

/**
 * Allows a member to return a checked out book
 */
void return_book(book_t **book_db, member_t **member_db){
    unsigned int *return_id = malloc(sizeof(int));
    printf("\nID of book to be returned: ");
    scanf("%u", return_id);
    clear_input_buffer();
    book_t *book = find_book(return_id,book_db);
    if(book == NULL){
        printf("Book to be returned not found; bookID %u, please try with another bookID.",*return_id);
        free(return_id);
        return;
    } else if(book->availablity == book->stock){
         printf("Book to be returned hasn't been issued; bookID %u, please try with another bookID.",*return_id);
        free(return_id);
        return;
    }
    printf("\nID of member who is returning the book: ");
    scanf("%u", return_id);
    clear_input_buffer();
    member_t *member = find_member(return_id,member_db);
    if(member == NULL){
        printf("Member returning the book not registered; memberID %u, please try with another memberID.",*return_id);
        free(return_id);
        return;
    }
    int *idx = malloc(sizeof(int));
    *idx = book_borrowed(book->bookID,member);
    if(*idx == -1){
        printf("Member has not borrowed this particular book registered; memberID %u, please try with another memberID.",*return_id);
        free(return_id);
        return;
    }
    update_member_borrow_history(idx,member);
    update_book_borrow_history(&member->memberID,book,member_db);
    printf("A copy of %s has been returned by %s.",book->title,member->name);
    free(return_id);
    free(idx);
}