#include <iostream>
#include <string>

#include "hashtable.h"

class MyHashFunction {
public:
    ulong operator()(const std::string &key) const {
        ulong value = 0;
        memcpy(&value, key.data(), key.length());
        return value % HashTableSize;
    }
};

int main() {
    HashTable<std::string, int, MyHashFunction> *ht = new HashTable<std::string, int, MyHashFunction>;

    ht->put("lel", 1);
    ht->put("kappa", 2);
    ht->put("lelelerinoe", 3);
    ht->put("lele", 4);

    std::cout << ht->get("lel") << "\n";

    return 0;
}
