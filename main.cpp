#include <iostream>
#include <string>

#include "hashtable.h"
#include "pointer.h"

int main() {
    //    HashTable<std::string, int> *ht = new HashTable<std::string, int>;

    //    ht->put("1", 1);
    //    ht = MemoryManager::shiftPointer(ht);
    //    ht->put("2", 2);
    //    ht = MemoryManager::shiftPointer(ht);
    //    ht->put("3", 3);
    //    ht = MemoryManager::shiftPointer(ht);
    //    ht->put("4", 4);
    //    ht = MemoryManager::shiftPointer(ht);

    //    std::cout << ht->get("3") << "\n";

    Pointer ht = new HashTable<std::string, int>;

    ((HashTable<std::string, int> *)*ht)->put("1", 1);
    ((HashTable<std::string, int> *)*ht)->put("2", 2);
    ((HashTable<std::string, int> *)*ht)->put("3", 3);
    ((HashTable<std::string, int> *)*ht)->put("4", 4);
    ((HashTable<std::string, int> *)*ht)->put("5", 5);
    //    ((HashTable<std::string, int> *) *ht)->put("6", 6);
    //    ((HashTable<std::string, int> *) *ht)->put("7", 7);
    //    ((HashTable<std::string, int> *) *ht)->put("8", 8);
    //    ((HashTable<std::string, int> *) *ht)->put("9", 9);
    //    ((HashTable<std::string, int> *) *ht)->put("10", 10);
    //    ((HashTable<std::string, int> *) *ht)->put("11", 11);

    //    std::cout << ((HashTable<std::string, int> *) *ht)->get("3") << "\n";

    return 0;
}
