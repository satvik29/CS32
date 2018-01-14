// set.h

#ifndef SET_H
#define SET_H

#include <string>

typedef std::string ItemType;
//typedef unsigned long ItemType;

class Set
{
public:
    Set();
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
    
    ~Set(); // destructor
    Set& operator= (const Set& other); // assignment operator
    Set(const Set& other); // copy constructor
    
private:
    struct Node {            // creating the node for the linked list
        ItemType m_value;
        Node* m_next;
        Node* m_prev;
    };
    int m_size; // contains the size of the linked list
    Node* m_head; // enables us to find the first node in the linked list
};

void unite(const Set& s1, const Set& s2, Set& result);

void subtract(const Set& s1, const Set& s2, Set& result);

#endif /* SET_H */
