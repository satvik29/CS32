// SymbolTable.cpp

// This is a correct but inefficient implementation of
// the SymbolTable functionality.

#include "SymbolTable.h"
#include <string>
#include <vector>
#include <stack>
#include <list>
using namespace std;

// This class does the real work of the implementation.

const int HASH_TABLE_SIZE = 20000;

struct Node {
    Node(string id, int lineNum) {    // This node is keeps track of the IDs and the linenumbers at which
        m_id = id;                    // items are added
        m_lineNum = lineNum;
    }
    string m_id;
    int m_lineNum;
};

struct Bucket {
    vector<Node> m_nodes;
};

// Each level of scope will have an id and a boolean indicating whether that scope is the first scope
struct Scope {
    vector<string> ids;
};

class SymbolTableImpl {
public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    Bucket m_buckets[HASH_TABLE_SIZE]; // array of m_buckets that is the hashtable
    int hashFunc(const string &id) const; // hash function that maps the string ID to a bucket
    stack<Scope> m_scopes; // stack of scopes to see which scope the program is in
    bool firstInsert = false;
};


int SymbolTableImpl::hashFunc(const string& id) const {
    int total = 0;
    for(int i = 0; i < id.size(); i++) {
        total = total + (i + 1) * id[i]; // multiply total at every iteration with the ascii value of
                                         // id[i]
    }
    return total % HASH_TABLE_SIZE;
}

void SymbolTableImpl::enterScope() {
    Scope s;
    
    if (m_scopes.empty()) {
        firstInsert = true;
    }
    
    m_scopes.push(s); // creates a new level of scope
}

bool SymbolTableImpl::exitScope() {
    if ((!firstInsert) && (m_scopes.empty() || m_scopes.size() == 1)) { // if we're at the 0th level of scope
        return false;
    }
    
    for (int i = 0; i < m_scopes.top().ids.size(); i++) { // removes corressponding nodes from buckets
        int hashValue = hashFunc(m_scopes.top().ids[i]);
        m_buckets[hashValue].m_nodes.pop_back();
    }
    
    
    m_scopes.pop(); // deletes scope
    
    return true;
}

bool SymbolTableImpl::declare(const string& id, int lineNum) {
    if (m_scopes.size() == 0) { // creates the first layer of scope
//        enterScope();
        Scope s;
        m_scopes.push(s);
    }
    
    for (int i = 0; i < m_scopes.top().ids.size(); i++) {
        if (m_scopes.top().ids.empty()) { // if empty, we can add straightaway
            break;
        }
        
        if (id == m_scopes.top().ids[i]) { // if the ID already exists, we cannot add
            return false;
        }
    }
    
    m_scopes.top().ids.push_back(id);
    
    Node temp(id, lineNum);
    m_buckets[hashFunc(id)].m_nodes.push_back(temp); // record in the hash table
    
    return true;
}

int SymbolTableImpl::find(const string& id) const {
    if (id.empty()) // if the string is empty return -1
        return -1;
    
    // this loop loops through the hash table to search for the required value using the hash function
    int hashValue = hashFunc(id);
    size_t i = m_buckets[hashValue].m_nodes.size();
    while (i > 0) {
        i--;
        if (m_buckets[hashValue].m_nodes[i].m_id == id) {
            return m_buckets[hashValue].m_nodes[i].m_lineNum;
        }
    }
    
    return -1;
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
    m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
    delete m_impl;
}

void SymbolTable::enterScope()
{
    m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
    return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
    return m_impl->find(id);
}



