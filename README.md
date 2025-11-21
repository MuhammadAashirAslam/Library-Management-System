# Library Management System

A full-stack Library Management System with a C++ backend and web-based frontend. This project demonstrates the integration of data structures and algorithms with modern web technologies.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Data Structures Used](#data-structures-used)
- [Technologies](#technologies)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [API Endpoints](#api-endpoints)
- [Command Line Interface](#command-line-interface)
- [License](#license)

## Overview

This Library Management System provides a complete solution for managing books, users, and transactions. The system uses custom-implemented data structures in C++ for efficient data management and provides a RESTful API through Node.js for web-based access.

## Features

- **Book Management**: Add, update, delete, and search books by ISBN, title, or author
- **User Management**: Register users, update profiles, and track borrowing history
- **Transaction Tracking**: Record and monitor book borrowing and return transactions
- **Sorting Algorithms**: Merge sort for books/users, insertion sort for transactions
- **Search Functionality**: Case-insensitive search across multiple fields
- **Data Persistence**: CSV-based file storage for all entities
- **RESTful API**: Express.js server providing HTTP endpoints
- **Web Interface**: Modern, responsive UI for all operations

## Architecture

The system follows a three-tier architecture:

```
Frontend (HTML/CSS/JS) <-> Node.js Express Server <-> C++ Backend (lms.exe)
                                                            |
                                                      Data Files (CSV)
```

### Data Flow

1. Frontend sends HTTP requests to Express server
2. Express server spawns C++ executable with command-line arguments
3. C++ backend processes the command and outputs JSON to stdout
4. Express server captures the output and returns it as HTTP response
5. Frontend receives and displays the data

## Data Structures Used

### 1. Doubly Linked List (DLL)

**Used for**: Books and Users

**Implementation**: `DoublyLinkedList<T>` in `LinkedList.h`

**Features**:
- Bidirectional traversal with `next` and `prev` pointers
- O(1) insertion at head/tail
- O(n) search, delete, and update operations
- Merge sort implementation for O(n log n) sorting

**Advantages**:
- Efficient insertion/deletion at both ends
- Bidirectional traversal for flexible operations
- No fixed size limitation

### 2. Singly Linked List (SLL)

**Used for**: Transactions

**Implementation**: `SinglyLinkedList<T>` in `LinkedList.h`

**Features**:
- Unidirectional traversal with `next` pointer only
- O(1) insertion at head
- O(n) insertion at tail, search, and delete
- Insertion sort for chronological ordering

**Advantages**:
- Lower memory overhead (one pointer per node vs two)
- Suitable for append-heavy operations (transactions)
- Simpler implementation for sequential access

### 3. Node Structures

**Doubly Linked List Node**: `Node<T>` in `Node.h`
```cpp
template<typename T>
class Node {
    T data;
    Node<T>* next;
    Node<T>* prev;
};
```

**Singly Linked List Node**: `SLLNode<T>` in `Node.h`
```cpp
template<typename T>
class SLLNode {
    T data;
    SLLNode<T>* next;
};
```

### 4. Specialized Lists

- **BookList**: Extends `DoublyLinkedList<Book>` with ISBN-specific operations
- **UserList**: Extends `DoublyLinkedList<User>` with user-specific operations
- **Transaction List**: Uses `SinglyLinkedList<Transaction>` for chronological records

## Technologies

### Backend (C++)
- **Language**: C++11
- **Compiler**: g++ with `-std=c++11 -Wall -Wextra -O2`
- **Build System**: Make
- **Data Storage**: CSV files

### Frontend Server (Node.js)
- **Runtime**: Node.js
- **Framework**: Express.js
- **Dependencies**:
  - `express`: Web server framework
  - `cors`: Cross-origin resource sharing
  - `body-parser`: Request body parsing
  - `child_process`: Spawn C++ executable

### Frontend (Web)
- **HTML5**: Structure
- **CSS3**: Styling
- **JavaScript**: Client-side logic
- **Fetch API**: HTTP requests

## Project Structure

```
OEL_DSA/
├── cpp_backend/
│   ├── Book.h                 # Book class with JSON serialization
│   ├── User.h                 # User class with borrowing logic
│   ├── Transaction.h          # Transaction class with timestamp
│   ├── Node.h                 # Node structures (DLL and SLL)
│   ├── LinkedList.h           # Generic linked list implementations
│   ├── BookList.h             # Specialized book list
│   ├── UserList.h             # Specialized user list
│   ├── LibrarySystem.cpp      # Main system logic and CLI
│   ├── Makefile               # Build configuration
│   └── lms.exe                # Compiled executable
├── data/
│   ├── books.txt              # Book records (CSV)
│   ├── users.txt              # User records (CSV)
│   └── transactions.txt       # Transaction records (CSV)
├── frontend/
│   ├── server.js              # Express server (API bridge)
│   ├── package.json           # Node.js dependencies
│   └── public/
│       ├── index.html         # Web interface
│       └── styles.css         # Styling
└── README.md                  # This file
```

## Installation

### Prerequisites

- **C++ Compiler**: g++ with C++11 support
- **Node.js**: Version 14 or higher
- **npm**: Node package manager
- **Make**: Build automation tool (optional)

### Step 1: Clone the Repository

```bash
git clone https://github.com/yourusername/library-management-system.git
cd library-management-system
```

### Step 2: Build C++ Backend

#### Using Make (Linux/Mac/WSL)
```bash
cd cpp_backend
make
```

#### Using g++ directly (Windows/Linux/Mac)
```bash
cd cpp_backend
g++ -std=c++11 -Wall -Wextra -O2 -o lms LibrarySystem.cpp
```

On Windows, the output will be `lms.exe`. On Linux/Mac, it will be `lms`.

### Step 3: Install Node.js Dependencies

```bash
cd ../frontend
npm install
```

### Step 4: Create Data Directory

```bash
cd ..
mkdir -p data
```

The data files will be created automatically on first run.

## Usage

### Running the Web Application

1. **Start the Node.js server**:
```bash
cd frontend
npm start
```

The server will start on `http://localhost:3001`

2. **Access the web interface**:
Open your browser and navigate to `http://localhost:3001`

### Using the C++ Backend Directly

You can also use the C++ backend directly from the command line:

```bash
cd cpp_backend
./lms get_books_json
```

## API Endpoints

### Books

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/books` | Get all books |
| POST | `/api/books` | Add a new book |
| GET | `/api/books/:isbn` | Get book by ISBN |
| PUT | `/api/books/:isbn` | Update book by ISBN |
| DELETE | `/api/books/:isbn` | Delete book by ISBN |
| GET | `/api/books/search/:query` | Search books by title |
| GET | `/api/sort/books/:criteria` | Sort books (title, author, publicationyear, isbn, availablecopies, totalcopies) |

### Users

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/users` | Get all users |
| POST | `/api/users` | Add a new user |
| GET | `/api/users/:userId` | Get user by ID |
| PUT | `/api/users/:userId` | Update user by ID |
| DELETE | `/api/users/:userId` | Delete user by ID |
| GET | `/api/users/search/:query` | Search users by name |
| GET | `/api/sort/users/:criteria` | Sort users (name, userid, email, phone) |

### Transactions

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/transactions` | Get all transactions |
| POST | `/api/transactions/borrow` | Borrow a book |
| POST | `/api/transactions/return` | Return a book |
| GET | `/api/transactions/user/:userId` | Get user transactions |
| GET | `/api/sort/transactions/:criteria` | Sort transactions (timestamp, userid, bookisbn, type) |

### Request/Response Examples

#### Add a Book
```bash
POST /api/books
Content-Type: application/json

{
  "isbn": "978-0134685991",
  "title": "Effective Modern C++",
  "author": "Scott Meyers",
  "publicationYear": 2014,
  "copies": 5
}
```

#### Borrow a Book
```bash
POST /api/transactions/borrow
Content-Type: application/json

{
  "userId": "U001",
  "bookISBN": "978-0134685991"
}
```

## Command Line Interface

The C++ backend can be used independently via command-line:

### Book Commands

```bash
# Get all books as JSON
./lms get_books_json

# Add a book
./lms add_book "978-0134685991" "Effective Modern C++" "Scott Meyers" 2014 5

# Delete a book
./lms delete_book "978-0134685991"

# Update a book
./lms update_book "978-0134685991" "New Title" "Author" 2024 10

# Search by ISBN
./lms search_book_isbn "978-0134685991"

# Search by title
./lms search_book_title "Modern C++"

# Search by author
./lms search_book_author "Meyers"

# Display all books
./lms display_books

# Sort books by title
./lms sort_books
```

### User Commands

```bash
# Get all users as JSON
./lms get_users_json

# Add a user
./lms add_user "U001" "John Doe" "john@example.com" "1234567890"

# Delete a user
./lms delete_user "U001"

# Update a user
./lms update_user "U001" "Jane Doe" "jane@example.com" "0987654321"

# Search by user ID
./lms search_user "U001"

# Search by name
./lms search_user_name "John"

# Display all users
./lms display_users

# Sort users by name
./lms sort_users
```

### Transaction Commands

```bash
# Get all transactions as JSON
./lms get_transactions_json

# Borrow a book
./lms borrow_book "U001" "978-0134685991"

# Return a book
./lms return_book "U001" "978-0134685991"

# Get user transactions
./lms user_transactions "U001"

# Display all transactions
./lms display_transactions

# Sort transactions by date
./lms sort_transactions
```

## Algorithm Complexity

### Doubly Linked List Operations

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Insert at Head/Tail | O(1) | O(1) |
| Delete by ID | O(n) | O(1) |
| Search by ID | O(n) | O(1) |
| Search by Title/Name | O(n) | O(k) where k = results |
| Merge Sort | O(n log n) | O(log n) |
| Display All | O(n) | O(1) |

### Singly Linked List Operations

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Insert at Head | O(1) | O(1) |
| Insert at Tail | O(n) | O(1) |
| Delete by ID | O(n) | O(1) |
| Search by ID | O(n) | O(1) |
| Insertion Sort | O(n²) | O(1) |
| Display All | O(n) | O(1) |

## Data File Format

### books.txt
```csv
ISBN,Title,Author,PublicationYear,TotalCopies,AvailableCopies
978-0134685991,Effective Modern C++,Scott Meyers,2014,5,3
```

### users.txt
```csv
UserID,Name,Email,Phone,BooksBorrowed,BorrowedISBNs
U001,John Doe,john@example.com,1234567890,2,978-0134685991;978-0321563842
```

### transactions.txt
```csv
TransactionID,UserID,BookISBN,TransactionType,Timestamp
TXN001,U001,978-0134685991,BORROW,2024-01-15 10:30:00
```

## Development

### Building from Source

```bash
cd cpp_backend
make clean
make
```

### Running Tests

```bash
cd cpp_backend
make test
```

### Development Mode (with auto-reload)

```bash
cd frontend
npm run dev
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Authors

- DSA-LMS Team

## Acknowledgments

- Data Structures and Algorithms course project
- C++ Standard Template Library documentation
- Express.js documentation
- Node.js community
