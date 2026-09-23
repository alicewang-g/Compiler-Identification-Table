Data Structures:

The implementation uses two main data structures working together: an open hash table and a 2D vector of strings to keep track of the data. The hash table is declared as: vector<Node*> m_table;. Each slot in the vector holds the head pointer of a singly-linked list of Node objects. The Nodes are a bit different than normal with a vector that keeps track of the line number each index is from and the amount of collisions there are.

    struct Node {
        string id;
        vector<pair<int,int>> stack;  // {lineNum, scopeDepth}
        Node* next;
    };

The other buckets are all initialized to nullptr.

To look up an identifier, we hash its name to get a bucket index, then walk the linked list at that bucket comparing names until we find a match or hit nullptr.

The hash function uses std::hash<string> % MAX to get the string in ints. A hash table reduces the average search time complexity to O(1) compared to the O(N) of the original slow one. 

—----

The scope stack is declared as: vector<vector<string>> m_scopes;. Each inner vector holds the names of identifiers declared at that scope level and the outer vector is affected by the exit and enter scope functions. The reason this data structure is also important is for when exitScope() is called, we need to know exactly which identifiers to undo. Rather than scanning the entire hash
table (which would be O(total identifiers)), we keep this list so we can undo only the declarations that belong to the current scope.


Time Complexity:

Let:
  n = total number of currently active identifiers across all scopes
  k = number of identifiers declared in the current scope
  b = number of identifiers that hash to the same bucket (collision chain length)

enterScope():
O(1) — just pushes an empty vector onto m_scopes. Same as the slow implementation.

exitScope():
O(k) where k is the number of identifiers declared in the scope being exited. For each name we do an O(1) average hash lookup and O(1) pop. The slow implementation is also O(k) for this operation.

declare():
O(1) average — one hash computation, O(b) bucket chain walk (O(1) average),
one stack peek for the duplicate check, one push. The slow implementation is O(k) because it scans all identifiers in the current scope linearly. 

find():
O(1) average — one hash computation, O(b) bucket chain walk (O(1) average), one peek at the back of the Node's stack. The slow implementation is O(n) because it scans all active identifiers
from the back of the vector. 

Compared to the original slow one, my implementation was more than 100 times faster using a test size of 200000 lines.

