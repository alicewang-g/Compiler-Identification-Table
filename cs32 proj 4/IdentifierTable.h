//
//  IdentifierTable.h
//  cs32 proj 4
//
//  Created by Alice Wang on 5/30/26.
//

#ifndef IDENTIFIERTABLE_INCLUDED
#define IDENTIFIERTABLE_INCLUDED

#include <string>

class IdentifierTableImpl;

class IdentifierTable
{
  public:
    IdentifierTable();
    ~IdentifierTable();
    void enterScope();
    bool exitScope();
    bool declare(const std::string& id, int lineNum);
    int find(const std::string& id) const;
      // We prevent a IdentifierTable object from being copied or assigned
    IdentifierTable(const IdentifierTable&) = delete;
    IdentifierTable& operator=(const IdentifierTable&) = delete;

  private:
    IdentifierTableImpl* m_impl;
};

#endif // IDENTIFIERTABLE_INCLUDED
