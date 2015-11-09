#pragma once

#include <stdexcept>

#include "object.h"

#include <iostream>

template <typename K>
class HashFunction {
public:
    ulong operator()(const K &key) const;
};

static const int HashTableSize = 10;

template <typename K, typename V, typename F>
class HashTable : public Object {
    class HashNode : public Object {
        K key;
        V value;
        HashNode *next;

    public:
        HashNode(K key, V value)
            : key(key), value(value), next(0) {
        }

        K getKey() const {
            return key;
        }

        V getValue() const {
            return value;
        }

        void setValue(V value) {
            HashNode::value = value;
        }

        HashNode *getNext() const {
            return next;
        }

        void setNext(HashNode *next) {
            HashNode::next = next;
        }
    };

    F hashFunction;
    HashNode **table;

public:
    HashTable() {
        table = new HashNode *[HashTableSize]();
    }

    ~HashTable() {
        for (int i = 0; i < HashTableSize; i++) {
            HashNode *entry = table[i];

            while (entry != 0) {
                HashNode *prev = entry;
                entry = entry->getNext();
                delete prev;
            }

            table[i] = 0;
        }

        delete[] table;
    }

    V get(const K &key) const {
        ulong hashValue = hashFunction(key);
        HashNode *entry = table[hashValue];

        while (entry != 0) {
            if (entry->getKey() == key)
                return entry->getValue();

            entry = entry->getNext();
        }

        throw std::out_of_range("HashTable::get");
    }

    void put(const K &key, const V &value) {
        ulong hashValue = hashFunction(key);

        std::cout << "hash: " << hashValue << "\n";

        HashNode *prev = 0;
        HashNode *entry = table[hashValue];

        while (entry != 0 && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry != 0) {
            entry->setValue(value);
            return;
        }

        entry = new HashNode(key, value);

        if (prev == 0)
            table[hashValue] = entry;
        else
            prev->setNext(entry);
    }

    void remove(const K &key) {
        ulong hashValue = hashFunction(key);

        HashNode *prev = 0;
        HashNode *entry = table[hashValue];

        while (entry != 0 && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == 0)
            return;

        if (prev == 0)
            table[hashValue] = entry->getNext();
        else
            prev->setNext(entry->getNext());

        delete entry;
    }
};

template <typename K>
ulong HashFunction<K>::operator()(const K &key) const {
    return reinterpret_cast<const ulong &>(key) % HashTableSize;
}
