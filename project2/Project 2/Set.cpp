// Set.cpp

#include "Set.h"

// construct an empty Set
Set::Set() {
    m_size = 0;
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

// destructor for the Set
Set::~Set() {
    while (m_head->m_next != m_head)
    {
        Node* i = m_head->m_next;
        i->m_next->m_prev = m_head;
        m_head->m_next = i->m_next;
        delete i;
    }
    delete m_head;
}

// assignment operator
Set& Set::operator= (const Set& other) {
    if (this == &other) {
        return (*this);
    } else {
        Set temp(other);
        swap(temp);
        return *this;
    }
}

// copy constructor
Set::Set(const Set& other) {
    m_size = other.m_size;
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
    
     // goes through the  entire set and replaces each value with the corresponding value from other
    for (Node* p = other.m_head->m_next; p != other.m_head; p = p->m_next) {
        Node* q = new Node;
        q->m_value = p->m_value;
        q->m_next = m_head;
        q->m_prev = m_head->m_prev;
        m_head->m_prev->m_next = q;
        m_head->m_prev = q;
    }
}

// checks if the set is empty
bool Set::empty() const {
    if (m_size == 0) {
        return true;
    }
    return false;
}

// returns the size of the set
int Set::size() const {
    return m_size;
}

// inserts a value at the end of the set
bool Set::insert(const ItemType& value) {

    
    // trying to check if value already exists in the set
    for (Node* i = m_head->m_next; i != m_head; i = i->m_next) {
        if (i->m_value == value) {
            return false;
        }
    }
    
    Node* insertedNode = new Node;
    // if value doesn't exist in the set
    insertedNode->m_value = value;
    
    if (size() == 0) {
        insertedNode->m_next = m_head;
        insertedNode->m_prev = m_head;
        m_head->m_next = insertedNode;
        m_head->m_prev = insertedNode;
        m_size++;
    } else {
        Node *temp = m_head->m_prev;
        insertedNode->m_next = m_head;
        insertedNode->m_prev = temp;
        temp->m_next = insertedNode;
        m_head->m_prev = insertedNode;
        m_size++;
    }
    
    return true;
}

// erases the given value from the set
bool Set::erase(const ItemType& value) {
    
    Node* temp = m_head;
    for (int i = 0; i < size(); i++) {
        temp = temp->m_next;
        if (temp->m_value == value) {
            Node* q = temp;
            temp->m_prev->m_next = q->m_next;
            temp->m_next->m_prev = q->m_prev;
            delete q;
            m_size--;
            return true;
        }
    }
    return false;
}

// checks if the given value is contained within the set
bool Set::contains(const ItemType& value) const {
    for (Node* i = m_head->m_next; i != m_head; i = i->m_next) {
        if (i->m_value == value) {
            return true;
        }
    }
    
    return false;
}

// if pos >= 0 and pos < size(), this function replaces value with the value in the set that is greater than exactly pos number of items.
bool Set::get(int pos, ItemType& value) const {
    if (pos >= 0 && pos < size()) {
        Node* i;
        Node* j;
        for (i = m_head->m_next; i != m_head; i = i->m_next) {
            int count = 0;
            for (j = m_head->m_next; j != m_head; j = j->m_next) {
                if (i->m_value > j->m_value) {
                    count++;
                }
            }
            if (count == pos) {
                value = i->m_value;
                return true;
            }
        }
    }
    return false;
}

// exchanges two sets
void Set::swap(Set& other) {
    Node* tempHead = m_head;
    m_head = other.m_head;
    other.m_head = tempHead;
    
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

// combines the items of two sets not including the repeated items
void unite(const Set& s1, const Set& s2, Set& result) {
    
    Set temp(result); // new variable incase of aliasing
    
    ItemType value1;
    for(int i = 0; i < s1.size(); i++) {
        s1.get(i, value1);
        temp.insert(value1);
    }
    
    ItemType value2;
    for (int j = 0; j < s1.size(); j++) {
        s2.get(j, value2);
        temp.insert(value2);
    }
    
    result = temp;
}

// displays only the values of s1 minus the common ones between s1 and s2
void subtract(const Set& s1, const Set& s2, Set& result) {
    
    Set temp(result); // new variable incase of aliasing
    
    // two temporary values
    ItemType valueI;
    ItemType valueJ;
    
    bool isCommon;
    
    // loops through the entire set
    for (int i = 0; i < s1.size(); i++) {
        isCommon = false;
        s1.get(i, valueI);
        
        // finds common values
        for (int j = 0; j < s2.size(); j++) {
            s2.get(j, valueJ);
            if (valueI == valueJ) {
                isCommon = true;
                break;
            }
        }
        
        // inserts if not common
        if (isCommon == false) {
            temp.insert(valueI);
        }
    }
    
    result = temp;
}

