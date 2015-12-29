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
        Pointer next;

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
            return (HashNode *)*(Pointer &)next;
        }

        void setNext(HashNode *next) {
            HashNode::next = next;
        }

        //        void shiftPointers() {
        //            Object::shiftPointers();

        //            if (next)
        //                next = MemoryManager::shiftPointer(next);
        //        }

        int getSize() {
            return sizeof(*this);
        }
    };

    static const int HashTableSize = 10;

    F hashFunction;
    Pointer table[HashTableSize];

public:
    HashTable()
    /*: table{0}*/ {
    }

    //    ~HashTable() {
    //        for (int i = 0; i < HashTableSize; i++) {
    //            Pointer entry = table[i];

    //            while (entry) {
    //                Pointer prev = entry;
    //                entry = ((HashNode *)*entry)->getNext();
    //                delete prev;
    //            }

    //            table[i] = 0;
    //        }
    //    }

    V get(const K &key) const {
        std::cout << "HashTable::get{this=" << this << ", key=" << key << "}\n";

        ulong hashValue = hashFunction(key) % HashTableSize;
        Pointer entry = table[hashValue];

        while (entry) {
            if (((HashNode *)(Object *) entry)->getKey() == key)
                return ((HashNode *)*entry)->getValue();

            entry = ((HashNode *)*entry)->getNext();
        }

        throw std::out_of_range("HashTable::get");
    }

    void put(const K &key, const V &value) {
        std::cout << "HashTable::put{this=" << this << ", key=" << key << ", value=" << value << "}\n";

        ulong hashValue = hashFunction(key) % HashTableSize;

        Pointer prev = 0;
        Pointer entry = table[hashValue];

        while (entry && ((HashNode *)*entry)->getKey() != key) {
            prev = entry;
            entry = ((HashNode *)*entry)->getNext();
        }

        if (entry) {
            ((HashNode *)*entry)->setValue(value);
            return;
        }

        Pointer _this = this;

        entry = new HashNode(key, value);

        //        HashTable<K, V, F> *newThis = MemoryManager::shiftPointer(this);

        //        if (prev)
        //            prev = MemoryManager::shiftPointer(prev);

        if (!prev)
            ((HashTable<K, V, F> *)*_this)->table[hashValue] = entry;
        else
            ((HashNode *)(Object *) prev)->setNext((HashNode *)(Object *) entry);
    }

    void remove(const K &key) {
        std::cout << "HashTable::remove{this=" << this << ", key=" << key << "}\n";

        ulong hashValue = hashFunction(key) % HashTableSize;

        Pointer prev = 0;
        Pointer entry = table[hashValue];

        while (entry && ((HashNode *)*entry)->getKey() != key) {
            prev = entry;
            entry = ((HashNode *)*entry)->getNext();
        }

        if (!entry)
            return;

        if (!prev)
            table[hashValue] = ((HashNode *)*entry)->getNext();
        else
            ((HashNode *)(Object *) prev)->setNext(((HashNode *)*entry)->getNext());

        delete entry;
    }

    //    void shiftPointers() {
    //        Object::shiftPointers();

    //        for (int i = 0; i < HashTableSize; i++)
    //            if (table[i])
    //                table[i] = MemoryManager::shiftPointer(table[i]);
    //    }

    int getSize() {
        return sizeof(*this);
    }
};
