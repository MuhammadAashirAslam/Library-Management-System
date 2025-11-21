# Makefile for Library Management System

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = lms
SOURCES = LibrarySystem.cpp
HEADERS = Book.h User.h Transaction.h Node.h LinkedList.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Run tests
test: $(TARGET)
	@echo "Testing book operations..."
	./$(TARGET) add_book "9780134685991" "Effective Modern C++" "Scott Meyers" 2014 5
	./$(TARGET) add_book "9780321563842" "C++ Primer" "Stanley Lippman" 2012 3
	./$(TARGET) display_books
	
	@echo "\nTesting user operations..."
	./$(TARGET) add_user "U001" "John Doe" "john@example.com" "1234567890"
	./$(TARGET) add_user "U002" "Jane Smith" "jane@example.com" "0987654321"
	./$(TARGET) display_users
	
	@echo "\nTesting borrow/return operations..."
	./$(TARGET) borrow_book "U001" "9780134685991"
	./$(TARGET) return_book "U001" "9780134685991"

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build the executable (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  test     - Run basic tests"
	@echo "  help     - Show this help message"

.PHONY: all clean test help