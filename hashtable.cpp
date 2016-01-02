#include "hashtable.h"

#include <stdexcept>

#include "memorymanager.h"
#include "pointer.h"

#include <iostream>

template class HashTable<std::string, Object *>;

template <class K, class V>
HashTable<K, V>::HashNode::HashNode(K key, V value)
    : key(key), value(value), next(0) {
}

template <class K, class V>
K &HashTable<K, V>::HashNode::getKey() {
    return key;
}

template <class K, class V>
V &HashTable<K, V>::HashNode::getValue() {
    return value;
}

template <class K, class V>
void HashTable<K, V>::HashNode::setValue(V value) {
    HashNode::value = value;
}

template <class K, class V>
typename HashTable<K, V>::HashNode *HashTable<K, V>::HashNode::getNext() const {
    return next;
}

template <class K, class V>
void HashTable<K, V>::HashNode::setNext(HashTable<K, V>::HashNode *next) {
    HashNode::next = next;
}

template <class K, class V>
void HashTable<K, V>::HashNode::shiftPointers(int delta) {
    Object::shiftPointers(delta);

    if (next)
        MemoryManager::shiftPointer(next, delta);
}

template <>
void HashTable<std::string, Object *>::HashNode::shiftPointers(int delta) {
    Object::shiftPointers(delta);

    if (next)
        MemoryManager::shiftPointer(next, delta);

    if (value)
        MemoryManager::shiftPointer(value, delta);
}

template <class K, class V>
void HashTable<K, V>::HashNode::mark() {
    Object::mark();

    if (next && !next->isMarked())
        next->mark();
}

template <>
void HashTable<std::string, Object *>::HashNode::mark() {
    Object::mark();

    if (next && !next->isMarked())
        next->mark();

    if (value && !value->isMarked())
        value->mark();
}

template <class K, class V>
int HashTable<K, V>::HashNode::getSize() {
    return sizeof *this;
}

template <class K, class V>
typename HashTable<K, V>::iterator &HashTable<K, V>::iterator::operator++() {
    if (node->getNext())
        node = node->getNext();
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

template <class K, class V>
typename HashTable<K, V>::iterator &HashTable<K, V>::iterator::operator*() {
    return *this;
}

template <class K, class V>
bool HashTable<K, V>::iterator::operator!=(const HashTable<K, V>::iterator &other) const {
    return node != other.node;
}

template <class K, class V>
K &HashTable<K, V>::iterator::key() {
    return node->getKey();
}

template <class K, class V>
V &HashTable<K, V>::iterator::value() {
    return node->getValue();
}

template <class K, class V>
HashTable<K, V>::iterator::iterator(HashTable<K, V>::HashNode **table)
    : table(table), i(0), node(0) {
}

template <class K, class V>
HashTable<K, V>::iterator::iterator(HashTable<K, V>::HashNode **table, int i)
    : table(table), i(i), node(table[i]) {
}

template <class K, class V>
HashTable<K, V>::HashTable()
    : table{0} {
}

template <class K, class V>
HashTable<K, V>::~HashTable() {
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

template <class K, class V>
typename HashTable<K, V>::iterator HashTable<K, V>::begin() {
    for (int i = 0; i < HashTableSize; i++)
        if (table[i])
            return iterator(table, i);

    return iterator(table);
}

template <class K, class V>
typename HashTable<K, V>::iterator HashTable<K, V>::end() {
    return iterator(table);
}

template <class K, class V>
V HashTable<K, V>::get(const K &key) const {
    ulong hashValue = hashFunction(key) % HashTableSize;
    HashNode *entry = table[hashValue];

    while (entry) {
        if (entry->getKey() == key)
            return entry->getValue();

        entry = entry->getNext();
    }

    throw std::out_of_range("HashTable<K, V>::get");
}

template <class K, class V>
void HashTable<K, V>::put(const K &key, const V &value) {
    std::cout << "HashTable<K, V>::put(key=" << key << ", value=" << value << ")\n";

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

    entry->shiftPointers(MemoryManager::getDelta());

    if (!prev)
        _this->table[hashValue] = entry;
    else
        prev->setNext(entry);
}

template <class K, class V>
void HashTable<K, V>::remove(const K &key) {
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
}

template <class K, class V>
bool HashTable<K, V>::contains(const K &key) {
    ulong hashValue = hashFunction(key) % HashTableSize;

    HashNode *entry = table[hashValue];

    while (entry && entry->getKey() != key)
        entry = entry->getNext();

    return entry;
}

template <class K, class V>
void HashTable<K, V>::shiftPointers(int delta) {
    Object::shiftPointers(delta);

    for (int i = 0; i < HashTableSize; i++)
        if (table[i])
            MemoryManager::shiftPointer(table[i], delta);
}

template <class K, class V>
void HashTable<K, V>::mark() {
    Object::mark();

    for (int i = 0; i < HashTableSize; i++)
        if (table[i] && !table[i]->isMarked())
            table[i]->mark();
}

template <class K, class V>
int HashTable<K, V>::getSize() {
    return sizeof *this;
}
