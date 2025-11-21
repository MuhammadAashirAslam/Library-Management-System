#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <fstream>
#include <string>

template<typename T>
class Node {
private:
    T data;
    Node<T>* next;
    Node<T>* prev;  // For Doubly Linked List

public:
    // Constructor
    Node() : next(nullptr), prev(nullptr) {}
    Node(const T& data) : data(data), next(nullptr), prev(nullptr) {}
    Node(const T& data, Node<T>* next) : data(data), next(next), prev(nullptr) {}
    Node(const T& data, Node<T>* next, Node<T>* prev) : data(data), next(next), prev(prev) {}

    // Getters
    T& getData() { return data; }
    const T& getData() const { return data; }
    Node<T>* getNext() const { return next; }
    Node<T>* getPrev() const { return prev; }

    // Setters
    void setData(const T& data) { this->data = data; }
    void setNext(Node<T>* next) { this->next = next; }
    void setPrev(Node<T>* prev) { this->prev = prev; }

    // File I/O operations
    void writeToFile(std::ofstream& file) const {
        data.writeToFile(file);
    }

    bool readFromFile(std::ifstream& file) {
        return data.readFromFile(file);
    }
};

// Singly Linked List Node (for Transactions)
template<typename T>
class SLLNode {
private:
    T data;
    SLLNode<T>* next;

public:
    // Constructor
    SLLNode() : next(nullptr) {}
    SLLNode(const T& data) : data(data), next(nullptr) {}
    SLLNode(const T& data, SLLNode<T>* next) : data(data), next(next) {}

    // Getters
    T& getData() { return data; }
    const T& getData() const { return data; }
    SLLNode<T>* getNext() const { return next; }

    // Setters
    void setData(const T& data) { this->data = data; }
    void setNext(SLLNode<T>* next) { this->next = next; }

    // File I/O operations
    void writeToFile(std::ofstream& file) const {
        data.writeToFile(file);
    }

    bool readFromFile(std::ifstream& file) {
        return data.readFromFile(file);
    }
};

#endif // NODE_H