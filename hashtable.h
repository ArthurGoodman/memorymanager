#pragma once

#include "object.h"

template <class K, class V>
class HashTable : public Object {
    class Entry : public Object {
        K key;
        V value;
        Entry *next;

    public:
        Entry(const K &key, const V &value);

        K &getKey();
        V &getValue();

        void setKey(const K &key);
        void setValue(const V &value);

        Entry *getNext() const;
        void setNext(Entry *next);

        bool equals(const K &key) const;

        void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
        int getSize();
    };

    static const int HashTableSize = 10;

    std::hash<K> hashFunction;
    Entry *table[HashTableSize];

public:
    class iterator {
        friend class HashTable;

        Entry **table;

        int i;
        Entry *entry;

    public:
        iterator &operator++();
        iterator &operator*();

        bool operator!=(const iterator &other) const;

        K &key();
        V &value();

    private:
        iterator(Entry **table);
        iterator(Entry **table, int i);
    };

    HashTable();

    iterator begin();
    iterator end();

    V get(const K &key) const;
    void put(const K &key, const V &value);
    bool remove(const K &key);
    bool contains(const K &key) const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize();

private:
    Entry *createEntry(const K &key, const V &value) const;
    ulong hash(const K &key) const;
};
