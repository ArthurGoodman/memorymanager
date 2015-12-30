#pragma once

#include <stdexcept>

#include "object.h"

template <class K, class V>
class HashTable : public Object {
    class HashNode : public Object {
        friend class HashTable::iterator;

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

            if (next)
                MemoryManager::shiftPointer(next, delta);
        }

        int getSize() {
            return sizeof *this;
        }
    };

    static const int HashTableSize = 10;

    std::hash<K> hashFunction;
    HashNode *table[HashTableSize];

public:
    HashTable()
        : table{0} {
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

        Pointer<HashTable<K, V>> _this = this;

        entry = new HashNode(key, value);

        entry->shiftPointers(MemoryManager::instance()->getDelta());

        if (!prev)
            _this->table[hashValue] = entry;
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

        delete *entry;
    }

    bool contains(const K &key) {
        for (auto i : *this)
            if (key == i.first)
                return true;

        return false;
    }

    void shiftPointers(int delta) {
        Object::shiftPointers(delta);

        for (int i = 0; i < HashTableSize; i++)
            if (table[i])
                MemoryManager::shiftPointer(table[i], delta);
    }

    int getSize() {
        return sizeof *this;
    }

    class iterator {
        friend class HashTable;

        HashNode **table;

        int i;
        HashNode *node;

    public:
        iterator &operator++() {
            if (node->next)
                node = node->next;
            else {
                i++;

                while (i < HashTableSize) {
                    if (table[i]) {
                        node = table[i];
                        break;
                    }

                    i++;
                }

                if (i == HashTableSize)
                    node = 0;
            }

            return *this;
        }

        std::pair<K, V> operator*() const {
            return std::make_pair(node->key, node->value);
        }

        bool operator!=(const iterator &other) const {
            return node != other.node;
        }

    private:
        iterator(HashNode **table)
            : table(table), i(0), node(0) {
        }

        iterator(HashNode **table, int i)
            : table(table), i(i), node(table[i]) {
        }
    };

    iterator begin() {
        for (int i = 0; i < HashTableSize; i++)
            if (table[i])
                return iterator(table, i);

        return iterator(table);
    }

    iterator end() {
        return iterator(table);
    }
};

template <>
inline void HashTable<std::string, Object *>::HashNode::shiftPointers(int delta) {
    Object::shiftPointers(delta);

    if (next)
        MemoryManager::shiftPointer(next, delta);

    if (value)
        MemoryManager::shiftPointer(value, delta);
}
