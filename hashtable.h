#pragma once

#include <stdexcept>

#include "object.h"
#include "memorymanager.h"

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

        void shiftPointers(int delta) {
            Object::shiftPointers(delta);

            if (next) {
                next = MemoryManager::shiftPointer(next);
                next->shiftPointers(delta);
            }
        }

        int getSize() {
            return sizeof(HashNode);
        }
    };

    static const int HashTableSize = 10;

    F hashFunction;
    HashNode *table[HashTableSize];

public:
    HashTable() {
        //table = new HashNode *[HashTableSize]();

        for (int i = 0; i < HashTableSize; i++)
            table[i] = 0;
    }

    ~HashTable() {
        for (int i = 0; i < HashTableSize; i++) {
            HashNode *entry = table[i];

            while (entry) {
                HashNode *prev = entry;
                entry = entry->getNext();
                delete prev;
            }

            table[i] = 0;
        }

        //delete[] table;
    }

    V get(const K &key) const {
        ulong hashValue = hashFunction(key) % HashTableSize;
        HashNode *entry = table[hashValue];

        while (entry) {
            if (entry->getKey() == key)
                return entry->getValue();

            entry = entry->getNext();
        }

        throw std::out_of_range("HashTable::get");
    }

    void put(const K &key, const V &value) {
        ulong hashValue = hashFunction(key) % HashTableSize;

        HashNode *prev = 0;
        HashNode *entry = table[hashValue];

        while (entry && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry) {
            entry->setValue(value);
            return;
        }

        entry = new HashNode(key, value);

        if (prev)
            prev = MemoryManager::shiftPointer(prev);

        if (!prev)
            table[hashValue] = entry;
        else
            prev->setNext(entry);
    }

    void remove(const K &key) {
        ulong hashValue = hashFunction(key) % HashTableSize;

        HashNode *prev = 0;
        HashNode *entry = table[hashValue];

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

        delete entry;
    }

    void shiftPointers(int delta) {
        Object::shiftPointers(delta);

        //table += delta;

        for (int i = 0; i < HashTableSize; i++) {
            if (table[i]) {
                table[i] = MemoryManager::shiftPointer(table[i]);

                HashNode *entry = table[i];

                while (entry) {
                    entry->shiftPointers(delta);
                    entry = entry->getNext();
                }
            }
        }
    }

    int getSize() {
        return sizeof(HashTable);
    }
};
