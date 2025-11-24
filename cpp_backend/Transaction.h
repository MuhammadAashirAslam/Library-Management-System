#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>

class Transaction 
{
private:
    std::string transactionId;
    std::string userId;
    std::string bookISBN;
    std::string transactionType; 
    std::string timestamp;

public:
    // Default constructor
    Transaction() {}

    // Parameterized constructor
    Transaction(const std::string& transactionId, const std::string& userId, 
                const std::string& bookISBN, const std::string& transactionType)
        : transactionId(transactionId), userId(userId), bookISBN(bookISBN), 
          transactionType(transactionType) {
        // Generate timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        // Convert to local time and format as string
        std::tm* local_time = std::localtime(&time_t);
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
        timestamp = std::string(buffer);
    }

    // Getters
    std::string getTransactionId() const { return transactionId; }
    std::string getUserId() const { return userId; }
    std::string getBookISBN() const { return bookISBN; }
    std::string getTransactionType() const { return transactionType; }
    std::string getTimestamp() const { return timestamp; }

    // Setters
    void setTransactionId(const std::string& transactionId) { this -> transactionId = transactionId; }
    void setUserId(const std::string& userId) { this->userId = userId; }
    void setBookISBN(const std::string& bookISBN) { this->bookISBN = bookISBN; }
    void setTransactionType(const std::string& transactionType) { 
        this->transactionType = transactionType; 
    }

    // Transaction-specific operations
    bool isBorrowTransaction() const {
        return transactionType == "BORROW";
    }

    bool isReturnTransaction() const {
        return transactionType == "RETURN";
    }
    // File I/O operations
    void writeToFile(std::ofstream& file) const {
        if (file.is_open()) {
            file << transactionId << "," 
                 << userId << "," 
                 << bookISBN << "," 
                 << transactionType << "," 
                 << timestamp << "\n";
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
            
            // Parse CSV format: transactionId,userId,bookISBN,transactionType,timestamp
            if ( std::getline(ss, token, ',') ) 
                transactionId = token;
            else 
                return false;
            
            if ( std::getline(ss, token, ',') ) 
                userId = token;
            else 
                return false;
            
            if ( std::getline(ss, token, ',') ) 
                bookISBN = token;
            else 
                return false;
            
            if ( std::getline(ss, token, ',') ) 
                transactionType = token;
            else 
                return false;
            
            if ( std::getline(ss, token, ',') ) 
                timestamp = token;
            else 
                return false;
            
            return true;
        }
        return false;
    }

    // Comparison operators for sorting (by timestamp, newest first)
    bool operator < (const Transaction& other) const {
        return timestamp > other.timestamp; // Reverse for newest first
    }

    bool operator > (const Transaction& other) const {
        return timestamp < other.timestamp; // Reverse for newest first
    }

    bool operator == (const Transaction& other) const {
        return transactionId == other.transactionId; // transactionId is unique identifier
    }

    // Display method
    void display() const {
        std::cout << "Transaction ID: " << transactionId << "\n"
                  << "User ID: " << userId << "\n "
                  << "Book ISBN: " << bookISBN << "\n "
                  << "Transaction Type: " << transactionType << "\n "
                  << "Timestamp: " << timestamp << "\n ";
    }

    // JSON-like string representation for frontend
    std::string toJSON() const {
        std::string json = "{";
        json += "\"transactionId\":\"" + transactionId + "\",";
        json += "\"userId\":\"" + userId + "\",";
        json += "\"bookISBN\":\"" + bookISBN + "\",";
        json += "\"transactionType\":\"" + transactionType + "\",";
        json += "\"timestamp\":\"" + timestamp + "\",";
        json += "\"isBorrowTransaction\":" + std::string(isBorrowTransaction() ? "true" : "false") + ",";
        json += "\"isReturnTransaction\":" + std::string(isReturnTransaction() ? "true" : "false");
        json += "}";
        return json;
    }

    // Static method to generate unique transaction ID
    static std::string generateTransactionId() {
        static int counter = 1;
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        return "TXN" + std::to_string(time_t) + std::to_string(counter++);
    }
};

#endif // TRANSACTION_H
