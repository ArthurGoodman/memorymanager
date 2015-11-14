#include <iostream>
#include <string>

#include "hashtable.h"

int main() {
    HashTable<std::string, int> *ht = new HashTable<std::string, int>;

    ht->put("1", 1);
    ht = MemoryManager::shiftPointer(ht);
    ht->put("2", 2);
    ht = MemoryManager::shiftPointer(ht);
    ht->put("3", 3);
    ht = MemoryManager::shiftPointer(ht);
    ht->put("4", 4);
    ht = MemoryManager::shiftPointer(ht);

    std::cout << ht->get("3") << "\n";

    return 0;
}
