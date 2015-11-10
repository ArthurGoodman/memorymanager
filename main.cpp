#include <iostream>
#include <string>

#include "hashtable.h"

int main() {
    HashTable<std::string, int> *ht = new HashTable<std::string, int>;

    ht->put("1", 11);
    ht->put("2", 12);
    ht->put("3", 13);
    ht->put("4", 14);

    std::cout << ht->get("3") << "\n";

    return 0;
}
