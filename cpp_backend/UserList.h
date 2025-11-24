#ifndef USERLIST_H
#define USERLIST_H

#include "LinkedList.h"
#include "User.h"

// Specialized Doubly Linked List for Users
class UserList : public DoublyLinkedList<User> {
public:
    bool deleteByUserId(const std::string& userId) {
        return deleteById(userId) ;
    }

    User* findByUserId(const std::string& userId) {
        return findById(userId) ;
    }

    bool updateByUserId(const std::string& userId, const User& newUser) {
        return updateById(userId, newUser) ;
    }

    std::vector<User> findByName(const std::string& searchTerm) {
        std::vector<User> results ;
        Node<User>* current = head ;
        std::string lowerSearch = toLower(searchTerm) ;
        
        while (current != nullptr) {
            std::string name = toLower(current->getData().getName());
            if (name.find(lowerSearch) != std::string::npos) {
                results.push_back(current->getData()) ;
            }
            current = current -> getNext() ;
        }
        return results ;
    }

private:
    std::string toLower(const std::string& str) const {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(c) ;
        }
        return result ;
    }
};

#endif // USERLIST_H
