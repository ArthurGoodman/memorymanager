#pragma once

#include "object.h"

template <class K, class V>
class HashTable : public Object {
    class HashNode : public Object {
        K key;
        V value;
        HashNode *next;

    public:
        HashNode(K key, V value);

        K &getKey();
        V &getValue();

        void setValue(V value);

        HashNode *getNext() const;
        void setNext(HashNode *next);

        void shiftPointers(int delta);
        void mark();

        int getSize();
    };

    static const int HashTableSize = 10;

    std::hash<K> hashFunction;
    HashNode *table[HashTableSize];

public:
    class iterator {
        friend class HashTable;

        HashNode **table;

        int i;
        HashNode *node;

    public:
        iterator &operator++();
        iterator &operator*();

        bool operator!=(const iterator &other) const;

        K &key();
        V &value();

    private:
        iterator(HashNode **table);
        iterator(HashNode **table, int i);
    };

    HashTable();
    ~HashTable();

    iterator begin();
    iterator end();

    V get(const K &key) const;
    void put(const K &key, const V &value);
    void remove(const K &key);
    bool contains(const K &key);

    void shiftPointers(int delta);
    void mark();

    int getSize();
};
