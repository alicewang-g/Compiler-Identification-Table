//
//  IdentifierTableslow.cpp
//  cs32 proj 4
//
//  Created by Alice Wang on 5/30/26.
//
// IdentifierTable.cpp

// This is a correct but inefficient implementation of
// the IdentifierTable functionality.

#include "IdentifierTable.h"
#include <string>
#include <vector>
#include <functional>
using namespace std;

  // This class does the real work of the implementation.

struct Node
{
    string id;
    vector<pair<int,int>> stack; // {lineNum, scopeDepth}
    Node* next;
    Node(const string& s) : id(s), next(nullptr) {}
};

class IdentifierTableImpl
{
  public:
    IdentifierTableImpl();
    ~IdentifierTableImpl();
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
  private:
    const int MAX = 10000;
    vector<Node*> m_table;
    vector<vector<string>> m_scopes;
    Node* findNode(const string& id) const{
        int b = bucketFor(id);
           Node* cur = m_table[b];
           while (cur != nullptr)
           {
               if (cur->id == id)
                   return cur;
               cur = cur->next;
           }
           return nullptr; // not found
    }
    int bucketFor(const string& id) const{
        return (int)(hash<string>{}(id) % MAX);
    } //helper
};

IdentifierTableImpl::IdentifierTableImpl(){
    m_table.resize(MAX,nullptr); //create table with max size
    m_scopes.push_back(vector<string>());
}

IdentifierTableImpl::~IdentifierTableImpl(){
    // Walk every bucket and delete its linked list
    for (int i = 0; i < MAX; i++){
        Node* cur = m_table[i];
        while (cur != nullptr){
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }
}

void IdentifierTableImpl::enterScope(){
    m_scopes.push_back(vector<string>());
}

//If m_scopes has only one entry (the global scope), return false —
//    you cannot exit a scope that was never entered.
//Otherwise,
//    walk through every name in m_scopes.back(),
//        find its Node in the hash table, and pop the back of that Node's stack (removing the current scope's declaration)
//Then pop m_scopes.back().


bool IdentifierTableImpl::exitScope(){
      // remove ids back to the last scope entry.
    if (m_scopes.size() <= 1)
        return false;

    for (const string& id : m_scopes.back()){
        Node* node = findNode(id);
        if (node != nullptr)
            node->stack.pop_back();
    }
    m_scopes.pop_back();
    return true;
}

//If id is empty, return false.
//    Hash id to find its bucket and walk the chain to find its Node.
//If found:
//    check whether the back of the Node's stack has the same scopeDepth as the current depth.
//    If so,
//        this is a duplicate declaration in the same scope — return false. Otherwise push {lineNum, depth} onto the Node's stack and record the name in m_scopes.back().
//    If not found: create a new Node, push {lineNum, depth} onto its stack, prepend it to the front of the bucket's linked list (O(1) prepend), and record the name in m_scopes.back().

bool IdentifierTableImpl::declare(const string& id, int lineNum){
    if (id.empty())
       return false;
   int depth = (int)m_scopes.size() - 1;
   Node* node = findNode(id);
   if (node != nullptr){
       // Identifier exists in hash table — check for duplicate in same scope
       if (!node->stack.empty() && node->stack.back().second == depth)
           return false;
       node->stack.push_back({lineNum, depth});
   }
   else{
       // New identifier — create a node and prepend it to the bucket's list
       int b = bucketFor(id);
       Node* newNode = new Node(id);
       newNode->stack.push_back({lineNum, depth});
       newNode->next = m_table[b];  // point to old head
       m_table[b] = newNode;        // new node becomes the head
   }
   m_scopes.back().push_back(id);
   return true;
}

//If id is empty, return -1.
//    Hash id, walk the bucket chain to find its Node.
//If not found or its stack is empty, return -1.
//Otherwise return stack.back().first (the lineNum of the innermost active declaration).

int IdentifierTableImpl::find(const string& id) const
{
    if (id.empty())
        return -1;
    Node* node = findNode(id);
    if (node == nullptr || node->stack.empty())
        return -1;
    return node->stack.back().first; // top of stack = most recent lineNum
}

//*********** IdentifierTable functions **************

// For the most part, these functions simply delegate to IdentifierTableImpl's
// functions.

IdentifierTable::IdentifierTable()
{
    m_impl = new IdentifierTableImpl;
}

IdentifierTable::~IdentifierTable()
{
    delete m_impl;
}

void IdentifierTable::enterScope()
{
    m_impl->enterScope();
}

bool IdentifierTable::exitScope()
{
    return m_impl->exitScope();
}

bool IdentifierTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int IdentifierTable::find(const string& id) const
{
    return m_impl->find(id);
}


