#ifndef USER_H
#define USER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

class User 
{
private:
    std::string userId ;
    std::string name ;
    std::string email ;
    std::string phone ; 
    int booksBorrowedCount ;
    std::vector<std::string> borrowedBooksISBNs ;

public:
    // Default constructor
    User() : booksBorrowedCount(0) {}

    // Constructor with just userId for search operations
    explicit User(const std::string& userId) : userId(userId), booksBorrowedCount(0) {}

    // Parameterized constructor
    User(const std::string& userId, const std::string& name, const std::string& email, const std::string& phone)
        : userId(userId), name(name), email(email), phone(phone), booksBorrowedCount(0) {}

    // Getters
    std::string getUserId() const 
    {
        return userId ;
    }

    std::string getName() const 
    { 
        return name ; 
    }
    
    std::string getEmail() const 
    { 
        return email ; 
    }
    
    std::string getPhone() const 
    { 
        return phone ;  
    }

    int getBooksBorrowedCount() const { return booksBorrowedCount; }
    const std::vector<std::string>& getBorrowedBooksISBNs() const return borrowedBooksISBNs; }

    // Setters
    void setUserId(const std::string& userId) { this->userId = userId; }
    void setName(const std::string& name) { this->name = name; }
    void setEmail(const std::string& email) { this->email = email; }
    void setPhone(const std::string& phone) { this->phone = phone; }

    // User-specific operations
    bool canBorrowBook() const {
        return booksBorrowedCount < 5; // Maximum 5 books per user
    }

    bool borrowBook(const std::string& isbn) {
        if (canBorrowBook() && booksBorrowedCount < 5) {
            borrowedBooksISBNs.push_back(isbn);
            booksBorrowedCount++ ;
            return true;
        }
        return false;
    }

    bool returnBook(const std::string& isbn) {
        for (auto it = borrowedBooksISBNs.begin(); it != borrowedBooksISBNs.end(); ++it) {
            if (*it == isbn) {
                borrowedBooksISBNs.erase(it);
                booksBorrowedCount--;
                return true;
            }
        }
        return false;
    }

    bool hasBorrowedBook(const std::string& isbn) const {
        for (const auto& borrowedISBN : borrowedBooksISBNs) {
            if (borrowedISBN == isbn) {
                return true;
            }
        }
        return false;
    }

    // File I/O operations
    void writeToFile(std::ofstream& file) const {
        if (file.is_open()) {
            file << userId << "," 
                 << name << "," 
                 << email << "," 
                 << phone << "," 
                 << booksBorrowedCount;
            
            // Write borrowed books ISBNs
            for (const auto& isbn : borrowedBooksISBNs) {
                file << "," << isbn;
            }
            file << "\n";
        }
    }

    bool readFromFile(std::ifstream& file) {
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        if (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            
            // Parse CSV format: userId,name,email,phone,booksBorrowedCount[,borrowedISBN1,borrowedISBN2,...]
            if (std::getline(ss, token, ',')) userId = token;
            else return false;
            
            if (std::getline(ss, token, ',')) name = token;
            else return false;
            
            if (std::getline(ss, token, ',')) email = token;
            else return false;
            
            if (std::getline(ss, token, ',')) phone = token;
            else return false;

            if (std::getline(ss, token, ',')) {
                bool numeric = true;
                for (char c : token) {
                    if (!std::isdigit(static_cast<unsigned char>(c)) && c != '-' && c != '+') { numeric = false; break; }
                }
                if (numeric && !token.empty()) {
                    booksBorrowedCount = std::stoi(token);
                } else {
                    // Skip optional password field then read count
                    if (std::getline(ss, token, ',')) {
                        booksBorrowedCount = std::stoi(token);
                    } else {
                        return false;
                    }
                }
            } else return false;
            
            // Read borrowed books ISBNs
            borrowedBooksISBNs.clear();
            while (std::getline(ss, token, ',')) {
                borrowedBooksISBNs.push_back(token);
            }
            
            return true;
        }
        return false;
    }

    // Comparison operators for sorting and searching
    bool operator<(const User& other) const {
        return name < other.name; // Sort by name by default
    }

    bool operator>(const User& other) const {
        return name > other.name;
    }

    bool operator==(const User& other) const {
        return userId == other.userId; // userId is unique identifier
    }

    // Display method
    void display() const {
        std::cout << "User ID: " << userId << "\n"
                  << "Name: " << name << "\n"
                  << "Email: " << email << "\n"
                  << "Phone: " << phone << "\n"
                  << "Books Borrowed: " << booksBorrowedCount << "/5\n";
        
        if (!borrowedBooksISBNs.empty()) {
            std::cout << "Borrowed Books ISBNs: ";
            for (size_t i = 0; i < borrowedBooksISBNs.size(); ++i) {
                std::cout << borrowedBooksISBNs[i];
                if (i < borrowedBooksISBNs.size() - 1) std::cout << ", ";
            }
            std::cout << "\n";
        }
    }

    // JSON-like string representation for frontend
    std::string toJSON() const {
        std::string json = "{";
        json += "\"userId\":\"" + userId + "\",";
        json += "\"name\":\"" + name + "\",";
        json += "\"email\":\"" + email + "\",";
        json += "\"phone\":\"" + phone + "\",";
        json += "\"booksBorrowedCount\":" + std::to_string(booksBorrowedCount) + ",";
        
        // Add borrowed books ISBNs
        json += "\"borrowedBooksISBNs\":[";
        for (size_t i = 0; i < borrowedBooksISBNs.size(); ++i) {
            json += "\"" + borrowedBooksISBNs[i] + "\"";
            if (i < borrowedBooksISBNs.size() - 1) json += ",";
        }
        json += "],";
        
        json += "\"canBorrowBook\":" + std::string(canBorrowBook() ? "true" : "false");
        json += "}";
        return json;
    }

};

#endif 
