#include "persist.h"
#include <stdio.h>
#include <stdlib.h>  
#include <stdint.h>

/**
 * Saves book data
 */
int save_books(book_t **book_db){
    FILE *fp = fopen(BOOKS_FILE,"wb"); 
    //wb -> Rewrites everything (write byte mode); ab->Appends new data ; (w->write text mode)
    //Since we will save and load, rewrite is fine
    if(!fp)
        return -1; //Failed to open file
    uint32_t magic = BOOK_MAGIC;
    fwrite(&magic, sizeof magic, 1, fp);
    book_t *head;
    for(int i=0;i<DB_SIZE;i++){
        if(book_db[i] == NULL)
            continue;
        head = malloc(sizeof(book_t));
        for(head = book_db[i];head;head=head->next){
            fwrite(&head->bookID, sizeof(head->bookID), 1, fp);
            fwrite(head->title, sizeof(char), string_length, fp);
            fwrite(head->author, sizeof(char), string_length, fp);
            fwrite(head->publications, sizeof(char), string_length, fp);
            fwrite(head->edition, sizeof(char), edition_length, fp);
            fwrite(&head->stock, sizeof(head->stock), 1, fp);
            fwrite(&head->availablity, sizeof(head->availablity), 1, fp);
            fwrite(head->closest_due_date, sizeof(char), date_length, fp);
            //Borrower List writing
            //First write count so we know when borrower IDs end
            int count = head->stock - head->availablity;
            fwrite(&count, sizeof(count), 1, fp);
            if (count > 0)
                fwrite(head->borrowers, sizeof(unsigned int), count, fp);
        }
    }
    free(head);
    fclose(fp);
    return 0;
}

/**
 * Saves member data
 */
int save_members(member_t **member_db){
    FILE *fp = fopen(MEMBERS_FILE,"wb");
    if(!fp)
        return -1;
    uint32_t magic = MEM_MAGIC;
    fwrite(&magic, sizeof magic, 1, fp);
    member_t *head;
    for(int i=0;i<DB_SIZE;i++){
        if(member_db[i] == NULL)
            continue;
        head = malloc(sizeof(member_t));
        for(head = member_db[i];head;head=head->next){
            fwrite(&head->memberID, sizeof(head->memberID), 1, fp);
            fwrite(head->name, sizeof(char), string_length, fp);
            fwrite(head->address, sizeof(char), add_length, fp);
            fwrite(head->phoneNo, sizeof(char), phone_length, fp);
            fwrite(head->email, sizeof(char), string_length, fp);
            //Write books_borrwed IDs and dueDate individually, by dereferencing
            for(int j = 0; j < 5; j++){
                unsigned int id = head->books_borrowed[j] ? *head->books_borrowed[j] : 0;
                fwrite(&id, sizeof id, 1, fp);

                char date[date_length] = {0};
                if(head->dueDates[j])
                    strncpy(date, head->dueDates[j], date_length - 1);
                fwrite(date, sizeof(char), date_length, fp);
            }
        }
    }
    free(head);
    fclose(fp);
    return 0;
}

/**
 * Load book data
 */
int load_books(book_t **book_db){
    FILE *fp = fopen(BOOKS_FILE,"rb");
    if(!fp)
        return 0;
    uint32_t magic;
    if(fread(&magic,sizeof(uint32_t),1,fp)!=1 || magic!=BOOK_MAGIC){
        fprintf(stderr, "Books file is missing, empty, or corrupt!\n");
        fclose(fp);
        return -1;
    }
    unsigned int id;
    while(fread(&id, sizeof(id), 1, fp) == 1){
        book_t *book = calloc(1,sizeof(book_t));
        if(!book)
            break;
        book->bookID = id;
        if(fread(book->title, sizeof(char), string_length, fp) != string_length||
        fread(book->author, sizeof(char), string_length, fp) !=  string_length||
        fread(book->publications, sizeof(char), string_length, fp) != string_length||
        fread(book->edition, sizeof(char), edition_length, fp)!= edition_length||
        fread(&book->stock, sizeof(book->stock), 1, fp) != 1||
        fread(&book->availablity, sizeof(book->availablity), 1, fp)!= 1||
        fread(book->closest_due_date, sizeof(char), date_length, fp) != date_length){
            free(book);
            break;
        }
        book->borrowers = calloc(book->stock,sizeof(unsigned int));
        int *count = malloc(sizeof(int));
        if(fread(count,sizeof(int),1,fp)!=1){
            free(book);
            break;
        }
        if(*count > 0 && fread(book->borrowers, sizeof(unsigned int), *count, fp) != *count){
            free(book->borrowers);
            free(book);
            break;
        }
        free(count);
        int *idx = malloc(sizeof(int));
        *idx = hash(book->bookID);
        book->next = book_db[*idx];
        book_db[*idx] = book;
        free(idx);
    }
    fclose(fp);
    return 0;
}

/**
 * Load member data
 */
int load_members(member_t **member_db){
    FILE *fp = fopen(MEMBERS_FILE,"rb");
    if(!fp)
        return 0;
    uint32_t magic;
    if(fread(&magic,sizeof(uint32_t),1,fp)!=1 || magic!=MEM_MAGIC){
        fprintf(stderr, "Members file is missing, empty, or corrupt!\n");
        fclose(fp);
        return -1;
    }
    unsigned int id;
    while(fread(&id, sizeof(id), 1, fp) == 1){
        member_t *member = calloc(1,sizeof(member_t));
        if(!member)
            break;
        member->memberID = id;
        if(fread(member->name,sizeof(char), string_length, fp)!=string_length||
        fread(member->address, sizeof(char), add_length, fp)!= add_length||
        fread(member->phoneNo, sizeof(char), phone_length, fp)!=phone_length||
        fread(member->email, sizeof(char), string_length, fp)!=string_length){
            free(member);
            break;
        }
        int *ok = malloc(sizeof(int));
        for(int i =0;i<5;i++){
            unsigned int id;
            char date[date_length];
            if(fread(&id, sizeof id, 1, fp) != 1 || fread(date, sizeof(char), date_length, fp) != date_length){
                *ok = 0;
                break;
            }
            if(id != 0){
                member->books_borrowed[i] = malloc(sizeof(unsigned int));
                member->dueDates[i] = malloc(date_length);
                if(!member->books_borrowed[i] || !member->dueDates[i]){
                    ok = 0;
                    break;
                }
                *member->books_borrowed[i] = id;
                memcpy(member->dueDates[i], date, date_length);
                member->dueDates[i][date_length - 1] = '\0';
            }
        }
        if(!*ok){
            for(int i =0;i<5;i++){
                free(member->books_borrowed[i]);
                free(member->dueDates[i]);
            }
            free(member);
            break;
        }
        free(ok);
        int *idx = malloc(sizeof(int));
        *idx = hash(member->memberID);
        member->next = member_db[*idx];
        member_db[*idx] = member;
        free(idx);
    }
    fclose(fp);
    return 0;
}

/**
 * Frees all allocated book and member data before exiting
 */
void free_all(book_t **book_db, member_t **member_db){
    for(int i=0;i<DB_SIZE;i++){
        //Free books
        book_t *book = book_db[i];
        book_t *next_book = malloc(sizeof(book_t));
        while(book){
            next_book = book->next;
            free(book->borrowers);
            free(book);
            book = next_book;
        }
        book_db[i] = NULL;
        free(next_book);

        //Free members
        member_t *member = member_db[i];
        member_t *next_member = malloc(sizeof(member_t));
        while(member){
            member_t *next = member->next;
            for(int j=0;j<5;j++){
                free(member->books_borrowed[j]);
                free(member->dueDates[j]);
            }
            free(member);
            member = next_member;
        }
        member_db[i]=NULL;
        free(next_member);
    }
}