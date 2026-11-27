# Library Management System

A console-based **Library Management System** written in C.  
Manage books and members, issue/return books, track due dates, and persist data to binary files.

## Features

- **Book Management**
  - Add, update, delete, and list books
  - View details of a particular book
  - Track total stock and current availability
  - Store title, author, publication house, and edition
  - Clean tabular display when listing books

- **Member Management**
  - Register, update, delete, and list members
  - View details of a particular member
  - Store name, address, phone number, and email
  - Each member can borrow up to **5 books** at a time
  - Tabular display of members and their borrowed books

- **Borrowing System**
  - Issue a book to a registered member
  - Return a borrowed book
  - Automatic **14-day due date** calculation
  - Check for books due today on startup
  - Prevents borrowing when stock is unavailable or the member already has a copy / has reached the limit

- **Data Persistence**
  - Saves and loads data to binary files (`books.dat` & `members.dat`)
  - Data is automatically loaded on start and saved on exit

- **Other**
  - Hash-table based storage (size 50) with linked-list collision handling
  - Input validation (empty strings, negative numbers, phone/email format, unique IDs)
  - Truncated string printing for clean table output
  - Support for piping input from a text file

## Project Structure

```
Library-Mgmnt-Sys/
├── book.c / book.h              # Book-related operations & table printing
├── member.c / member.h          # Member & borrow/return logic + table printing
├── lib_system.c / lib_system.h  # Main program, utilities & data structures
├── persist.c / persist.h        # Save/load to binary files
├── input.txt                    # Sample input file for non-interactive runs
├── .gitignore
└── README.md
```

## Data Structures

- `book_t` – book information, borrower list, and closest due date
- `member_t` – member information and list of currently borrowed books with due dates

Both are stored in hash tables of size `DB_SIZE` (50).

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

### Optional: Run with input from a file

```bash
./library input.txt
```

(The input file should be a plain text file containing menu choices and answers.)

## Usage

When you start the program you will see a menu:

```
1.  Add book to library
2.  Update existing book
3.  Delete book from library
4.  Get details of a particular book
5.  Get all books in library
6.  Register new member to library
7.  Update information of existing member of library
8.  Remove member from library
9.  Get details of a particular member
10. Get all members of library
11. Issue a book to member
12. Return a book borrowed by a member
e.  Exit application
```

- Enter the number of the action you want to perform.
- Type `e` (or `E`) to save data and exit.

Listing books or members prints a formatted table with headers and separators. Long titles/names are truncated for readability.

## Notes

- Data is stored in `books.dat` and `members.dat` in the same directory as the executable.
- On startup the program loads any existing data and checks for books due today.
- Maximum 5 books per member.
- Due date is automatically set to **14 days** from the issue date (`#define due_date 14`).
- Phone numbers must be exactly 10 digits. Email is optional but must contain `@` if provided.
