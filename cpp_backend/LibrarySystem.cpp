#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Book.h"
#include "User.h"
#include "Transaction.h"
#include "LinkedList.h"
#include "BookList.h"
#include "UserList.h"

class LibrarySystem {
private:
    BookList books;
    UserList users;
    SinglyLinkedList<Transaction> transactions;
    
    std::string booksFile = "../data/books.txt";
    std::string usersFile = "../data/users.txt";
    std::string transactionsFile = "../data/transactions.txt";

public:
    // Constructor - load data from files
    LibrarySystem() {
        loadData();
    }

    // Destructor - save data to files
    ~LibrarySystem() {
        saveData();
    }

    // Book operations
    bool addBook(const std::string& isbn, const std::string& title, const std::string& author, 
                 int publicationYear, int totalCopies) {
        // Check if book already exists
        if (books.findByISBN(isbn) != nullptr) {
            std::cout << "Book with ISBN " << isbn << " already exists.\n";
            return false;
        }

        Book newBook(isbn, title, author, publicationYear, totalCopies);
        books.insertAtHead(newBook);
        saveData();
        std::cout << "Book added successfully.\n";
        return true;
    }

    bool deleteBook(const std::string& isbn) {
        if (books.deleteByISBN(isbn)) {
            saveData();
            std::cout << "Book deleted successfully.\n";
            return true;
        } else {
            std::cout << "Book with ISBN " << isbn << " not found.\n";
            return false;
        }
    }

    bool updateBook(const std::string& isbn, const std::string& title, const std::string& author, 
                    int publicationYear, int totalCopies) {
        Book* book = books.findByISBN(isbn);
        if (book != nullptr) {
            book->setTitle(title);
            book->setAuthor(author);
            book->setPublicationYear(publicationYear);
            book->setTotalCopies(totalCopies);
            saveData();
            std::cout << "Book updated successfully.\n";
            return true;
        } else {
            std::cout << "Book with ISBN " << isbn << " not found.\n";
            return false;
        }
    }

    Book* searchBookByISBN(const std::string& isbn) {
        return books.findByISBN(isbn);
    }

    std::vector<Book> searchBooksByTitle(const std::string& title) {
        return books.findByTitle(title);
    }

    std::vector<Book> searchBooksByAuthor(const std::string& author) {
        std::vector<Book> results;
        auto allBooks = books.getAllData();
        std::string lowerAuthor = toLower(author);
        
        for (const auto& book : allBooks) {
            if (toLower(book.getAuthor()).find(lowerAuthor) != std::string::npos) {
                results.push_back(book);
            }
        }
        return results;
    }

    void displayAllBooks() const {
        books.displayAll();
    }

    void sortBooksByTitle() {
        books.mergeSort();
        saveData();
        std::cout << "Books sorted by title.\n";
    }

    // User operations
    bool addUser(const std::string& userId, const std::string& name, const std::string& email, const std::string& phone) {
        // Check if user already exists
        if (users.findByUserId(userId) != nullptr) {
            std::cout << "User with ID " << userId << " already exists.\n";
            return false;
        }

        User newUser(userId, name, email, phone);
        users.insertAtHead(newUser);
        saveData();
        std::cout << "User added successfully.\n";
        return true;
    }


    bool deleteUser(const std::string& userId) {
        // Check if user has borrowed books
        User* user = users.findByUserId(userId);
        if (user != nullptr && user->getBooksBorrowedCount() > 0) {
            std::cout << "Cannot delete user with borrowed books. Return all books first.\n";
            return false;
        }

        if (users.deleteByUserId(userId)) {
            saveData();
            std::cout << "User deleted successfully.\n";
            return true;
        } else {
            std::cout << "User with ID " << userId << " not found.\n";
            return false;
        }
    }

    bool updateUser(const std::string& userId, const std::string& name, const std::string& email, const std::string& phone) {
        User* user = users.findByUserId(userId);
        if (user != nullptr) {
            user->setName(name);
            user->setEmail(email);
            user->setPhone(phone);
            saveData();
            std::cout << "User updated successfully.\n";
            return true;
        } else {
            std::cout << "User with ID " << userId << " not found.\n";
            return false;
        }
    }

    User* searchUserById(const std::string& userId) {
        return users.findByUserId(userId);
    }

    std::vector<User> searchUsersByName(const std::string& name) {
        return users.findByName(name);
    }

    void displayAllUsers() const {
        users.displayAll();
    }

    void sortUsersByName() {
        users.mergeSort();
        saveData();
        std::cout << "Users sorted by name.\n";
    }

    // Transaction operations (Borrow/Return)
    bool borrowBook(const std::string& userId, const std::string& isbn) {
        User* user = users.findByUserId(userId);
        Book* book = books.findByISBN(isbn);

        if (user == nullptr) {
            std::cout << "User with ID " << userId << " not found.\n";
            return false;
        }

        if (book == nullptr) {
            std::cout << "Book with ISBN " << isbn << " not found.\n";
            return false;
        }

        if (!user->canBorrowBook()) {
            std::cout << "User has reached maximum borrowing limit (5 books).\n";
            return false;
        }

        if (!book->isAvailable()) {
            std::cout << "Book is not available for borrowing.\n";
            return false;
        }

        // Perform the borrow operation
        if (user->borrowBook(isbn) && book->borrowBook()) {
            std::string transactionId = Transaction::generateTransactionId();
            Transaction newTransaction(transactionId, userId, isbn, "BORROW");
            transactions.insertAtHead(newTransaction);
            saveData();
            std::cout << "Book borrowed successfully. Transaction ID: " << transactionId << "\n";
            return true;
        }

        return false;
    }

    bool returnBook(const std::string& userId, const std::string& isbn) {
        User* user = users.findByUserId(userId);
        Book* book = books.findByISBN(isbn);

        if (user == nullptr) {
            std::cout << "User with ID " << userId << " not found.\n";
            return false;
        }

        if (book == nullptr) {
            std::cout << "Book with ISBN " << isbn << " not found.\n";
            return false;
        }

        if (!user->hasBorrowedBook(isbn)) {
            std::cout << "User has not borrowed this book.\n";
            return false;
        }

        // Perform the return operation
        if (user->returnBook(isbn) && book->returnBook()) {
            std::string transactionId = Transaction::generateTransactionId();
            Transaction newTransaction(transactionId, userId, isbn, "RETURN");
            transactions.insertAtHead(newTransaction);
            saveData();
            std::cout << "Book returned successfully. Transaction ID: " << transactionId << "\n";
            return true;
        }

        return false;
    }

    std::vector<Transaction> getUserTransactions(const std::string& userId) {
        return transactions.findByUserId(userId);
    }

    void displayAllTransactions() const {
        transactions.displayAll();
    }

    void sortTransactionsByDate() {
        transactions.insertionSort();
        saveData();
        std::cout << "Transactions sorted by date.\n";
    }

    // Data persistence
    void loadData() {
        books.loadFromFile(booksFile);
        users.loadFromFile(usersFile);
        transactions.loadFromFile(transactionsFile);
        std::cout << "Data loaded successfully.\n";
    }

    void saveData() {
        books.saveToFile(booksFile);
        users.saveToFile(usersFile);
        transactions.saveToFile(transactionsFile);
    }


    // JSON output for frontend
    std::string getAllBooksJSON() const {
        auto allBooks = books.getAllData();
        std::string json = "[";
        for (size_t i = 0; i < allBooks.size(); ++i) {
            json += allBooks[i].toJSON();
            if (i < allBooks.size() - 1) json += ",";
        }
        json += "]";
        return json;
    }

    std::string getAllUsersJSON() const {
        auto allUsers = users.getAllData();
        std::string json = "[";
        for (size_t i = 0; i < allUsers.size(); ++i) {
            json += allUsers[i].toJSON();
            if (i < allUsers.size() - 1) json += ",";
        }
        json += "]";
        return json;
    }

    std::string getAllTransactionsJSON() const {
        auto allTransactions = transactions.getAllData();
        std::string json = "[";
        for (size_t i = 0; i < allTransactions.size(); ++i) {
            json += allTransactions[i].toJSON();
            if (i < allTransactions.size() - 1) json += ",";
        }
        json += "]";
        return json;
    }

private:
    std::string toLower(const std::string& str) const {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(c);
        }
        return result;
    }
};

// Command line interface
void printUsage() {
    std::cout << "Usage: ./lms [command] [arguments...]\n";
    std::cout << "Commands:\n";
    std::cout << "  add_book <isbn> <title> <author> <year> <copies>\n";
    std::cout << "  delete_book <isbn>\n";
    std::cout << "  update_book <isbn> <title> <author> <year> <copies>\n";
    std::cout << "  search_book_isbn <isbn>\n";
    std::cout << "  search_book_title <title>\n";
    std::cout << "  search_book_author <author>\n";
    std::cout << "  display_books\n";
    std::cout << "  sort_books\n";
    std::cout << "  add_user <userId> <name> <email> <phone>\n";
    std::cout << "  delete_user <userId>\n";
    std::cout << "  update_user <userId> <name> <email> <phone>\n";
    std::cout << "  search_user <userId>\n";
    std::cout << "  search_user_name <name>\n";
    std::cout << "  display_users\n";
    std::cout << "  sort_users\n";
    std::cout << "  borrow_book <userId> <isbn>\n";
    std::cout << "  return_book <userId> <isbn>\n";
    std::cout << "  user_transactions <userId>\n";
    std::cout << "  display_transactions\n";
    std::cout << "  sort_transactions\n";
    std::cout << "  get_books_json\n";
    std::cout << "  get_users_json\n";
    std::cout << "  get_transactions_json\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    LibrarySystem library;
    std::string command = argv[1];

    try {
        if (command == "add_book" && argc == 7) {
            std::string isbn = argv[2];
            std::string title = argv[3];
            std::string author = argv[4];
            int year = std::stoi(argv[5]);
            int copies = std::stoi(argv[6]);
            library.addBook(isbn, title, author, year, copies);

        } else if (command == "delete_book" && argc == 3) {
            std::string isbn = argv[2];
            library.deleteBook(isbn);

        } else if (command == "update_book" && argc == 7) {
            std::string isbn = argv[2];
            std::string title = argv[3];
            std::string author = argv[4];
            int year = std::stoi(argv[5]);
            int copies = std::stoi(argv[6]);
            library.updateBook(isbn, title, author, year, copies);

        } else if (command == "search_book_isbn" && argc == 3) {
            std::string isbn = argv[2];
            Book* book = library.searchBookByISBN(isbn);
            if (book != nullptr) {
                std::cout << book->toJSON() << "\n";
            } else {
                std::cout << "{}\n";
            }

        } else if (command == "search_book_title" && argc == 3) {
            std::string title = argv[2];
            auto results = library.searchBooksByTitle(title);
            std::cout << "[";
            for (size_t i = 0; i < results.size(); ++i) {
                std::cout << results[i].toJSON();
                if (i < results.size() - 1) std::cout << ",";
            }
            std::cout << "]\n";

        } else if (command == "search_book_author" && argc == 3) {
            std::string author = argv[2];
            auto results = library.searchBooksByAuthor(author);
            std::cout << "[";
            for (size_t i = 0; i < results.size(); ++i) {
                std::cout << results[i].toJSON();
                if (i < results.size() - 1) std::cout << ",";
            }
            std::cout << "]\n";

        } else if (command == "display_books" && argc == 2) {
            library.displayAllBooks();

        } else if (command == "sort_books" && argc == 2) {
            library.sortBooksByTitle();

        } else if (command == "add_user" && argc == 6) {
            std::string userId = argv[2];
            std::string name = argv[3];
            std::string email = argv[4];
            std::string phone = argv[5];
            library.addUser(userId, name, email, phone);

        } else if (command == "delete_user" && argc == 3) {
            std::string userId = argv[2];
            library.deleteUser(userId);

        } else if (command == "update_user" && argc == 6) {
            std::string userId = argv[2];
            std::string name = argv[3];
            std::string email = argv[4];
            std::string phone = argv[5];
            library.updateUser(userId, name, email, phone);

        } else if (command == "search_user" && argc == 3) {
            std::string userId = argv[2];
            User* user = library.searchUserById(userId);
            if (user != nullptr) {
                std::cout << user->toJSON() << "\n";
            } else {
                std::cout << "{}\n";
            }

        } else if (command == "search_user_name" && argc == 3) {
            std::string name = argv[2];
            auto results = library.searchUsersByName(name);
            std::cout << "[";
            for (size_t i = 0; i < results.size(); ++i) {
                std::cout << results[i].toJSON();
                if (i < results.size() - 1) std::cout << ",";
            }
            std::cout << "]\n";

        } else if (command == "display_users" && argc == 2) {
            library.displayAllUsers();

        } else if (command == "sort_users" && argc == 2) {
            library.sortUsersByName();

        } else if (command == "borrow_book" && argc == 4) {
            std::string userId = argv[2];
            std::string isbn = argv[3];
            library.borrowBook(userId, isbn);

        } else if (command == "return_book" && argc == 4) {
            std::string userId = argv[2];
            std::string isbn = argv[3];
            library.returnBook(userId, isbn);

        } else if (command == "user_transactions" && argc == 3) {
            std::string userId = argv[2];
            auto results = library.getUserTransactions(userId);
            std::cout << "[";
            for (size_t i = 0; i < results.size(); ++i) {
                std::cout << results[i].toJSON();
                if (i < results.size() - 1) std::cout << ",";
            }
            std::cout << "]\n";

        } else if (command == "display_transactions" && argc == 2) {
            library.displayAllTransactions();

        } else if (command == "sort_transactions" && argc == 2) {
            library.sortTransactionsByDate();

        } else if (command == "get_books_json" && argc == 2) {
            std::cout << library.getAllBooksJSON() << "\n";

        } else if (command == "get_users_json" && argc == 2) {
            std::cout << library.getAllUsersJSON() << "\n";

        } else if (command == "get_transactions_json" && argc == 2) {
            std::cout << library.getAllTransactionsJSON() << "\n";

        } else {
            std::cout << "Invalid command or incorrect number of arguments.\n";
            printUsage();
            return 1;
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}