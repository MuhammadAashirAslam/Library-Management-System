#ifndef BOOKLIST_H
#define BOOKLIST_H

#include "LinkedList.h"
#include "Book.h"

// Specialized Doubly Linked List for Books
class BookList : public DoublyLinkedList<Book> {
public:
    bool deleteByISBN(const std::string& isbn) {
        return deleteById(isbn);
    }

    Book* findByISBN(const std::string& isbn) {
        return findById(isbn);
    }

    bool updateByISBN(const std::string& isbn, const Book& newBook) {
        return updateById(isbn, newBook);
    }

    std::vector<Book> findByTitle(const std::string& searchTerm) {
        return findByTitleOrName(searchTerm);
    }
};

#endif // BOOKLIST_H