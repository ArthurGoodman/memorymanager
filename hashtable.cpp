#include "hashtable.h"

#include <stdexcept>

#include "memorymanager.h"
#include "pointer.h"

#include <iostream>

HashTable::HashNode::HashNode(std::string key, const Pointer<Object> value)
    : key(key), value(value), next(0) {
}

HashTable::HashNode::~HashNode() {
    std::cout << "HashTable::HashNode::~HashNode() //value=" << value << "\n";
}

std::string &HashTable::HashNode::getKey() {
    return key;
}

Object *&HashTable::HashNode::getValue() {
    return value;
}

void HashTable::HashNode::setValue(Object *value) {
    HashNode::value = value;
}

typename HashTable::HashNode *HashTable::HashNode::getNext() const {
    return next;
}

void HashTable::HashNode::setNext(HashTable::HashNode *next) {
    HashNode::next = next;
}

void HashTable::HashNode::shiftPointers() {
    Object::shiftPointers();

    if (next)
        MemoryManager::shiftPointer(next);

    if (value)
        MemoryManager::shiftPointer(value);
}

void HashTable::HashNode::forwardPointers() {
    Object::forwardPointers();

    if (next)
        next = (HashNode *)next->getForwardAddress();

    if (value)
        value = (Object *)value->getForwardAddress();
}

void HashTable::HashNode::mark() {
    Object::mark();

    if (next && !next->isMarked())
        next->mark();

    if (value && !value->isMarked())
        value->mark();
}

int HashTable::HashNode::getSize() {
    return sizeof *this;
}

typename HashTable::iterator &HashTable::iterator::operator++() {
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

typename HashTable::iterator &HashTable::iterator::operator*() {
    return *this;
}

bool HashTable::iterator::operator!=(const HashTable::iterator &other) const {
    return node != other.node;
}

std::string &HashTable::iterator::key() {
    return node->getKey();
}

Object *&HashTable::iterator::value() {
    return node->getValue();
}

HashTable::iterator::iterator(HashTable::HashNode **table)
    : table(table), i(0), node(0) {
}

HashTable::iterator::iterator(HashTable::HashNode **table, int i)
    : table(table), i(i), node(table[i]) {
}

HashTable::HashTable()
    : table{0} {
}

HashTable::~HashTable() {
    std::cout << "HashTable::~HashTable()\n";
}

typename HashTable::iterator HashTable::begin() {
    for (int i = 0; i < HashTableSize; i++)
        if (table[i])
            return iterator(table, i);

    return iterator(table);
}

typename HashTable::iterator HashTable::end() {
    return iterator(table);
}

Object *HashTable::get(const std::string &key) const {
    ulong hashValue = hashFunction(key) % HashTableSize;
    HashNode *entry = table[hashValue];

    while (entry) {
        if (entry->getKey() == key)
            return entry->getValue();

        entry = entry->getNext();
    }

    throw std::out_of_range("HashTable::get");
}

void HashTable::put(const std::string &key, const Pointer<Object> &value) {
    std::cout << "HashTable::put(key=" << key << ", value=" << value << ")\n";

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

    Pointer<HashTable> _this = this;

    entry = new HashNode(key, value);

    if (!prev)
        _this->table[hashValue] = entry;
    else
        prev->setNext(entry);
}

void HashTable::remove(const std::string &key) {
    std::cout << "HashTable::remove(key=" << key << ")\n";

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

bool HashTable::contains(const std::string &key) {
    ulong hashValue = hashFunction(key) % HashTableSize;

    HashNode *entry = table[hashValue];

    while (entry && entry->getKey() != key)
        entry = entry->getNext();

    return entry;
}

void HashTable::shiftPointers() {
    Object::shiftPointers();

    for (int i = 0; i < HashTableSize; i++)
        if (table[i])
            MemoryManager::shiftPointer(table[i]);
}

void HashTable::forwardPointers() {
    Object::forwardPointers();

    for (int i = 0; i < HashTableSize; i++)
        if (table[i])
            table[i] = (HashNode *)table[i]->getForwardAddress();
}

void HashTable::mark() {
    Object::mark();

    for (int i = 0; i < HashTableSize; i++)
        if (table[i] && !table[i]->isMarked())
            table[i]->mark();
}

int HashTable::getSize() {
    return sizeof *this;
}
