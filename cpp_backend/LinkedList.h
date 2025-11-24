#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <type_traits>

// Doubly Linked List for Books and Users (DLL)
template<typename T>
class DoublyLinkedList {
protected:
    Node<T>* head;
    Node<T>* tail;
private:
    int size;

public:
    // Constructor and Destructor
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~DoublyLinkedList() {
        clear();
    }

    // Basic operations
    void insertAtHead(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        
        if (head == nullptr) {
            head = tail = newNode;
        } 
        
        else {
            newNode->setNext(head);
            head->setPrev(newNode);
            head = newNode;
        }
        size++;
    }

    void insertAtTail(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        
        if (tail == nullptr) {
            head = tail = newNode;
        } 
        
        else {
            tail->setNext(newNode);
            newNode->setPrev(tail);
            tail = newNode;
        }
        size++;
    }

    bool deleteById(const std::string& id) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->getData() == T(id)) { // Assuming T can be constructed with id
                // Found the node to delete
                if (current->getPrev() != nullptr) {
                    current->getPrev()->setNext(current->getNext());
                } 
                
                else {
                    // Deleting head
                    head = current->getNext();
                }

                if (current->getNext() != nullptr) {
                    current->getNext()->setPrev(current->getPrev());
                } 
                
                else {
                    // Deleting tail
                    tail = current->getPrev();
                }

                delete current;
                size--;
                return true;
            }
            current = current -> getNext();
        }
        return false;
    }

    T* findById(const std::string& id) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->getData() == T(id)) { // Using constructor with id
                return &current->getData();
            }
            current = current->getNext();
        }
        return nullptr;
    }

    std::vector<T> findByTitleOrName(const std::string& searchTerm) {
        std::vector<T> results;
        Node<T>* current = head;
        std::string lowerSearch = toLower(searchTerm);
        
        while (current != nullptr) {
            std::string titleOrName = toLower(current->getData().getTitle());
            if (titleOrName.find(lowerSearch) != std::string::npos) {
                results.push_back(current->getData());
            }
            current = current -> getNext();
        }
        return results;
    }

    bool updateById(const std::string& id, const T& newData) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->getData() == T(id)) { // Using constructor with id
                current->setData(newData);
                return true;
            }
            current = current->getNext();
        }
        return false;
    }

    void displayAll() const {
        Node<T>* current = head;
        int count = 1;
        while (current != nullptr) {
            std::cout << "--- Item " << count << " ---\n";
            current->getData().display();
            std::cout << "\n";
            current = current->getNext();
            count++;
        }
        if (count == 1) {
            std::cout << "No items found.\n";
        }
    }

    std::vector<T> getAllData() const {
        std::vector<T> data ;
        Node<T>* current = head ;
        while (current != nullptr) {
            data.push_back(current->getData());
            current = current->getNext();
        }
        return data;
    }

    void clear() {
        Node<T>* current = head ;
        while (current != nullptr) {
            Node<T>* next = current->getNext();
            delete current;
            current = next;
        }
        head = tail = nullptr ;
        size = 0;
    }

    int getSize() const {
        return size;
    }

    bool isEmpty() const {
        return size == 0;
    }

    // Merge Sort implementation for O(N log N) complexity
    void mergeSort() {
        head = mergeSort(head) ;
        // Update tail
        tail = head;
        while (tail != nullptr && tail->getNext() != nullptr) {
            tail = tail->getNext() ;
        }
    }

    // File I/O operations
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename) ;
        if (file.is_open()) {
            Node<T>* current = head ;
            while (current != nullptr) {
                current->writeToFile(file) ;
                current = current->getNext() ;
            }
            file.close();
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename) ;
        if (file.is_open()) {
            clear() ;
            T data ;
            while (data.readFromFile(file)) {
                insertAtTail(data) ;
            }
            file.close() ;
        }
    }

private:
    std::string toLower(const std::string& str) const {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(c) ;
        }
        return result;
    }

    // Merge Sort helper functions
    Node<T>* mergeSort(Node<T>* head) {
        if (head == nullptr || head->getNext() == nullptr) {
            return head;
        }

        // Split the list into two halves
        Node<T>* second = split(head) ;

        // Recursively sort both halves
        head = mergeSort(head) ;
        second = mergeSort(second) ; 

        // Merge the sorted halves
        return merge(head, second) ;
    }

    Node<T>* split(Node<T>* head) {
        Node<T>* fast = head ;
        Node<T>* slow = head ;
        Node<T>* prev = nullptr ;

        while (fast != nullptr && fast->getNext() != nullptr) {
            prev = slow ;
            slow = slow -> getNext() ;
            fast = fast -> getNext() -> getNext() ;
        }

        if (prev != nullptr) {
            prev->setNext(nullptr);
        }

        return slow;
    }

    Node<T>* merge(Node<T>* first, Node<T>* second) {
        if (first == nullptr) 
            return second;
        if (second == nullptr) 
            return first;

        if (first->getData() < second->getData()) {
            first->setNext(merge(first->getNext(), second));
            
            if (first->getNext() != nullptr) {
                first->getNext()->setPrev(first);
            }
            
            first->setPrev(nullptr);
            return first;
        } 
        
        else {
            second->setNext(merge(first, second->getNext()));
            
            if (second->getNext() != nullptr) {
                second->getNext()->setPrev(second);
            }
            
            second->setPrev(nullptr);
            return second;
        }
    }
};

// Singly Linked List for Transactions (SLL)
template<typename T>
class SinglyLinkedList {
private:
    SLLNode<T>* head;
    int size;

public:
    // Constructor and Destructor
    SinglyLinkedList() : head(nullptr), size(0) {}
    
    ~SinglyLinkedList() {
        clear();
    }

    // Basic operations
    void insertAtHead(const T& data) {
        SLLNode<T>* newNode = new SLLNode<T>(data, head);
        head = newNode;
        size++;
    }

    void insertAtTail(const T& data) {
        SLLNode<T>* newNode = new SLLNode<T>(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            SLLNode<T>* current = head;
            while (current->getNext() != nullptr) {
                current = current->getNext();
            }
            current->setNext(newNode);
        }
        size++;
    }

    bool deleteById(const std::string& id) {
        if (head == nullptr) return false;

        if (head->getData().getTransactionId() == id) {
            SLLNode<T>* temp = head;
            head = head->getNext();
            delete temp;
            size--;
            return true;
        }

        SLLNode<T>* current = head;
        while (current->getNext() != nullptr) {
            if (current->getNext()->getData().getTransactionId() == id) {
                SLLNode<T>* temp = current->getNext();
                current->setNext(temp->getNext());
                delete temp;
                size--;
                return true;
            }
            current = current->getNext();
        }
        return false;
    }

    T* findById(const std::string& id) {
        SLLNode<T>* current = head;
        while (current != nullptr) {
            if (current->getData().getTransactionId() == id) {
                return &current->getData();
            }
            current = current->getNext();
        }
        return nullptr;
    }

    std::vector<T> findByUserId(const std::string& userId) {
        std::vector<T> results;
        SLLNode<T>* current = head;
        while (current != nullptr) {
            if (current->getData().getUserId() == userId) {
                results.push_back(current->getData());
            }
            current = current->getNext();
        }
        return results;
    }

    void displayAll() const {
        SLLNode<T>* current = head;
        int count = 1;
        while (current != nullptr) {
            std::cout << "--- Transaction " << count << " ---\n";
            current->getData().display();
            std::cout << "\n";
            current = current->getNext();
            count++;
        }
        if (count == 1) {
            std::cout << "No transactions found.\n";
        }
    }

    std::vector<T> getAllData() const {
        std::vector<T> data;
        SLLNode<T>* current = head;
        while (current != nullptr) {
            data.push_back(current->getData());
            current = current->getNext();
        }
        return data;
    }

    void clear() {
        SLLNode<T>* current = head;
        while (current != nullptr) {
            SLLNode<T>* next = current->getNext();
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
    }

    int getSize() const {
        return size;
    }

    bool isEmpty() const {
        return size == 0;
    }

    // Insertion sort for transactions (since they're mostly append-only)
    void insertionSort() {
        if (head == nullptr || head->getNext() == nullptr) return ;

        SLLNode<T>* sorted = nullptr ;
        SLLNode<T>* current = head ;

        while (current != nullptr) {
            SLLNode<T>* next = current->getNext() ;
            sortedInsert(current, sorted) ;
            current = next ;
        }

        head = sorted ;
    }

    // File I/O operations
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename) ;
        if (file.is_open()) {
            SLLNode<T>* current = head ;
            while (current != nullptr) {
                current->writeToFile(file) ;
                current = current->getNext() ;
            }
            file.close() ;
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            clear() ;
            T data ;
            while (data.readFromFile(file)) {
                insertAtTail(data) ;
            }
            file.close() ;
        }
    }

private:
    void sortedInsert(SLLNode<T>* newNode, SLLNode<T>*& sorted) {
        if (sorted == nullptr || newNode -> getData() < sorted -> getData()) {
            newNode->setNext(sorted) ;
            sorted = newNode ;
        } else {
            SLLNode<T>* current = sorted;
            while (current->getNext() != nullptr && 
                   !(newNode -> getData() < current -> getNext() -> getData() ) ) {
                current = current -> getNext();
            }
            newNode -> setNext(current -> getNext());
            current -> setNext(newNode);
        }
    }
};

#endif // LINKEDLIST_H
