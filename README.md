# Library Management System

A console-based **Library Management System** written in C.  
It lets you manage books and members, issue/return books, track due dates, and persist data to binary files.

## Features

- **Book Management**
  - Add, update, delete, and list books
  - Track total stock and current availability
  - Store title, author, publication house, and edition

- **Member Management**
  - Register, update, delete, and list members
  - Store name, address, phone number, and email
  - Each member can borrow up to **5 books** at a time

- **Borrowing System**
  - Issue a book to a registered member
  - Return a borrowed book
  - Automatic **14-day due date** calculation
  - Check for books due today on startup
  - Prevents borrowing when stock is unavailable or the member already has a copy

- **Data Persistence**
  - Saves and loads data to binary files (`books.dat` & `members.dat`)
  - Data is automatically loaded on start and saved on exit

- **Other**
  - Hash-table based storage (size 50) with linked-list collision handling
  - Input validation (empty strings, negative numbers, phone/email format, unique IDs)
  - Support for piping input from a file

## Project Structure

```
Library-Mgmnt-Sys/
├── book.c / book.h              # Book-related operations
├── member.c / member.h          # Member & borrow/return logic
├── lib_system.c / lib_system.h  # Main program, utilities & data structures
├── persist.c / persist.h        # Save/load to binary files
└── .gitignore
```

## Data Structures

- `book_t` – book information, borrower list, and closest due date
- `member_t` – member information and list of currently borrowed books with due dates

Both are stored in hash tables of size 50.

## Requirements

- A C compiler (GCC, Clang, or MSVC)
- Standard C library (`stdio`, `stdlib`, `string`, `time`, `stdbool`, `stdint`)

## How to Build & Run

### Linux / macOS

```bash
gcc -o library lib_system.c book.c member.c persist.c -std=c11
./library
```

### Windows (MinGW / GCC)

```bash
gcc -o library.exe lib_system.c book.c member.c persist.c -std=c11
library.exe
```

### Optional: Run with input from a file (Should be a txt file)

```bash
./library input.txt
```

## Usage

When you start the program you will see a menu:

```
1.  Add book to library
2.  Update existing book
3.  Delete book from library
4.  Get all books in library
5.  Register new member to library
6.  Update information of existing member
7.  Remove member from library
8.  Get all members of library
9.  Issue a book to member
10. Return a book borrowed by a member
e.  Exit application
```

- Enter the number of the action you want to perform.
- Type `e` (or `E`) to save data and exit.

## Notes

- Data is stored in `books.dat` and `members.dat` in the same directory as the executable.
- On startup the program loads any existing data and checks for books due today.
- Maximum 5 books per member.
- Due date is automatically set to **14 days** from the issue date.