#include <iostream>
#include <string>

#include "hashtable.h"
#include "pointer.h"

int main() {
    Pointer<HashTable<std::string, int>> ht = new HashTable<std::string, int>;

    ht->put("1", 1);
    ht->put("2", 2);
    ht->put("3", 3);
    ht->put("4", 4);
    ht->put("5", 5);

    std::cout << ht->get("3") << "\n";

    return 0;
}
