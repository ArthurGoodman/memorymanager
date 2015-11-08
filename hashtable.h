#pragma once

#include "list.h"

template <typename K, typename V>
class HashTable : public Object {
    class HashNode {
        K key;
        V value;
    };

    static const int TableSize = 10;

    List<HashNode> **table;
    int size;

public:
    HashTable()
        : size(0) {
        table = new List<HashNode> *[TableSize];

        for (int i = 0; i < TableSize; i++)
            table[i] = new List<HashNode>;
    }

    int getSize() {
        return size;
    }
};
