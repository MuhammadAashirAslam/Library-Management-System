#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Book 
{
private:
    std::string isbn ;
    std::string title ;
    std::string author ;
    int publicationYear ;
    int availableCopies ;
    int totalCopies ;

public:
    // Default constructor
    Book() : publicationYear(0), availableCopies(0), totalCopies(0) {}

    // Constructor with just ISBN for search operations
    explicit Book(const std::string& isbn) : isbn(isbn), publicationYear(0), availableCopies(0), totalCopies(0) {}

    // Parameterized constructor
    Book(const std::string& isbn, const std::string& title, const std::string& author, 
         int publicationYear, int totalCopies)
        : isbn(isbn), title(title), author(author), publicationYear(publicationYear), 
          availableCopies(totalCopies), totalCopies(totalCopies) {}

    // Getters
    std::string getISBN() const { 
        return isbn ; 
    }
    std::string getTitle() const { 
        return title ; 
    }
    std::string getAuthor() const { 
        return author ; 
    }
    
    int getPublicationYear() const { 
        return publicationYear ; 
    }

    int getAvailableCopies() const { 
        return availableCopies ; 
    }
    
    int getTotalCopies() const { 
        return totalCopies ; 
    }

    // Setters
    void setISBN(const std::string& isbn) { 
        this -> isbn = isbn ; 
    }
    
    void setTitle(const std::string& title) { 
        this -> title = title ; 
    }
    
    void setAuthor(const std::string& author) { 
        this -> author = author ; 
    }

    void setPublicationYear(int year) { 
        this -> publicationYear = year ; 
    }

    void setTotalCopies(int copies) { 
        this -> totalCopies = copies; 
        this -> availableCopies = copies ; 
    }

    // Book-specific operations
    bool borrowBook() {
        if (availableCopies > 0) {
            availableCopies-- ;
            return true ;
        }
        return false ;
    }

    bool returnBook() {
        if (availableCopies < totalCopies) {
            availableCopies++ ;
            return true ;
        }
        return false ;
    }

    bool isAvailable() const {
        return availableCopies > 0 ;
    }

    // File I/O operations
    void writeToFile(std::ofstream& file) const {
        if (file.is_open()) {
            file << isbn << "," 
                 << title << "," 
                 << author << "," 
                 << publicationYear << "," 
                 << totalCopies << "," 
                 << availableCopies << "\n";
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
            
            // Parse CSV format: ISBN,Title,Author,Year,TotalCopies,AvailableCopies
            if (std::getline(ss, token, ',')) isbn = token;
            else return false;
            
            if (std::getline(ss, token, ',')) title = token;
            else return false;
            
            if (std::getline(ss, token, ',')) author = token;
            else return false;
            
            if (std::getline(ss, token, ',')) publicationYear = std::stoi(token);
            else return false;
            
            if (std::getline(ss, token, ',')) totalCopies = std::stoi(token);
            else return false;
            
            if (std::getline(ss, token, ',')) availableCopies = std::stoi(token);
            else return false;
            
            return true;
        }
        return false;
    }

    // Comparison operators for sorting and searching
    bool operator<(const Book& other) const {
        return title < other.title; // Sort by title by default
    }

    bool operator>(const Book& other) const {
        return title > other.title;
    }

    bool operator==(const Book& other) const {
        return isbn == other.isbn; // ISBN is unique identifier
    }

    // Display method
    void display() const {
        std::cout << "ISBN:. " << isbn << "\n"
                  << "Title:. " << title << "\n"
                  << "Author:. " << author << "\n"
                  << "Publication Year:. " << publicationYear << "\n"
                  << "Available Copies:. " << availableCopies << "/" << totalCopies << "\n"
                  << "Status:. " << (isAvailable() ? "Available" : "Not Available") << "\n" ;
    }

    // JSON-like string representation for frontend
    std::string toJSON() const {
        std::string json = "{";
        json += "\"isbn\":\"" + isbn + "\",";
        json += "\"title\":\"" + title + "\",";
        json += "\"author\":\"" + author + "\",";
        json += "\"publicationYear\":" + std::to_string(publicationYear) + ",";
        json += "\"availableCopies\":" + std::to_string(availableCopies) + ",";
        json += "\"totalCopies\":" + std::to_string(totalCopies) + ",";
        json += "\"isAvailable\":" + std::string(isAvailable() ? "true" : "false");
        json += "}";
        return json;
    }
};

#endif // BOOK_H
