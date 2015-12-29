#pragma once

#include <stdexcept>

#include "object.h"
#include "memorymanager.h"
#include "pointer.h"

#include <iostream>

template <typename K, typename V, typename F = std::hash<K>>
class HashTable : public Object {
    class HashNode : public Object {
        K key;
        V value;
        Pointer<HashNode> next;

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

        int getSize() {
            return sizeof *this;
        }
    };

    static const int HashTableSize = 10;

    F hashFunction;
    Pointer<HashNode> table[HashTableSize];

public:
    ~HashTable() {
        for (int i = 0; i < HashTableSize; i++) {
            Pointer<HashNode> entry = table[i];

            while (entry) {
                Pointer<HashNode> prev = entry;
                entry = entry->getNext();
                delete prev;
            }

            table[i] = 0;
        }
    }

    V get(const K &key) const {
        std::cout << "HashTable::get{this=" << this << ", key=" << key << "}\n";

        ulong hashValue = hashFunction(key) % HashTableSize;
        Pointer<HashNode> entry = table[hashValue];

        while (entry) {
            if (entry->getKey() == key)
                return entry->getValue();

            entry = entry->getNext();
        }

        throw std::out_of_range("HashTable::get");
    }

    void put(const K &key, const V &value) {
        std::cout << "HashTable::put{this=" << this << ", key=" << key << ", value=" << value << "}\n";

        ulong hashValue = hashFunction(key) % HashTableSize;

        Pointer<HashNode> prev = 0;
        Pointer<HashNode> entry = table[hashValue];

        while (entry && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry) {
            entry->setValue(value);
            return;
        }

        Pointer<HashTable<K, V, F>> _this = this;

        entry = new HashNode(key, value);

        if (!prev)
            _this->table[hashValue] = entry;
        else
            prev->setNext(entry);
    }

    void remove(const K &key) {
        std::cout << "HashTable::remove{this=" << this << ", key=" << key << "}\n";

        ulong hashValue = hashFunction(key) % HashTableSize;

        Pointer<HashNode> prev = 0;
        Pointer<HashNode> entry = table[hashValue];

        while (entry && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (!entry)
            return;

        if (!prev)
            table[hashValue] = entry->getNext();
        else
            prev->setNext(entry->getNext());

        delete *entry;
    }

    int getSize() {
        return sizeof *this;
    }
};
