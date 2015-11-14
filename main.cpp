#include <iostream>
#include <string>

#include "hashtable.h"

int main() {
    HashTable<std::string, int> *ht = new HashTable<std::string, int>;

    ht->put("1", 1);
    ht->put("2", 2);
    ht->put("3", 3);
    ht->put("4", 4);

    std::cout << ht->get("2") << "\n";

    return 0;
}
