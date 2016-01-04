#include "hashtable.h"

#include <stdexcept>

#include "imemorymanager.h"
#include "pointer.h"

#include <iostream>

template class HashTable<std::string, Object *>;

template <class K, class V>
HashTable<K, V>::Entry::Entry(const K &key, const V &value)
    : key(key), value(value), next(0) {
}

template <class K, class V>
K &HashTable<K, V>::Entry::getKey() {
    return key;
}

template <class K, class V>
V &HashTable<K, V>::Entry::getValue() {
    return value;
}

template <class K, class V>
void HashTable<K, V>::Entry::setValue(const V &value) {
    Entry::value = value;
}

template <class K, class V>
typename HashTable<K, V>::Entry *HashTable<K, V>::Entry::getNext() const {
    return next;
}

template <class K, class V>
void HashTable<K, V>::Entry::setNext(HashTable<K, V>::Entry *next) {
    Entry::next = next;
}

template <class K, class V>
void HashTable<K, V>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    if (next)
        f((ManagedObject *&)next);

    if (value)
        f((ManagedObject *&)value);
}

template <class K, class V>
int HashTable<K, V>::Entry::getSize() {
    return sizeof *this;
}

template <class K, class V>
typename HashTable<K, V>::iterator &HashTable<K, V>::iterator::operator++() {
    if (entry->getNext())
        entry = entry->getNext();
    else {
        i++;

        while (i < HashTableSize) {
            if (table[i]) {
                entry = table[i];
                break;
            }

            i++;
        }

        if (i == HashTableSize)
            entry = 0;
    }

    return *this;
}

template <class K, class V>
typename HashTable<K, V>::iterator &HashTable<K, V>::iterator::operator*() {
    return *this;
}

template <class K, class V>
bool HashTable<K, V>::iterator::operator!=(const HashTable<K, V>::iterator &other) const {
    return entry != other.entry;
}

template <class K, class V>
K &HashTable<K, V>::iterator::key() {
    return entry->getKey();
}

template <class K, class V>
V &HashTable<K, V>::iterator::value() {
    return entry->getValue();
}

template <class K, class V>
HashTable<K, V>::iterator::iterator(HashTable<K, V>::Entry **table)
    : table(table), i(0), entry(0) {
}

template <class K, class V>
HashTable<K, V>::iterator::iterator(HashTable<K, V>::Entry **table, int i)
    : table(table), i(i), entry(table[i]) {
}

template <class K, class V>
HashTable<K, V>::HashTable()
    : table{0} {
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
    Entry *entry = table[hashValue];

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

    Pointer<Entry> prev;
    Pointer<Entry> entry = table[hashValue];

    while (entry && entry->getKey() != key) {
        prev = entry;
        entry = entry->getNext();
    }

    if (entry) {
        entry->setValue(value);
        return;
    }

    Pointer<HashTable> _this = this;

    entry = createEntry(key, value);

    if (!prev)
        _this->table[hashValue] = entry;
    else
        prev->setNext(entry);
}

template <class K, class V>
bool HashTable<K, V>::remove(const K &key) {
    std::cout << "HashTable<K, V>::remove(key=" << key << ")\n";

    ulong hashValue = hashFunction(key) % HashTableSize;

    Entry *prev = 0;
    Entry *entry = table[hashValue];

    while (entry && entry->getKey() != key) {
        prev = entry;
        entry = entry->getNext();
    }

    if (!entry)
        return false;

    if (!prev)
        table[hashValue] = entry->getNext();
    else
        prev->setNext(entry->getNext());

    return true;
}

template <class K, class V>
bool HashTable<K, V>::contains(const K &key) {
    ulong hashValue = hashFunction(key) % HashTableSize;

    Entry *entry = table[hashValue];

    while (entry && entry->getKey() != key)
        entry = entry->getNext();

    return entry;
}

template <class K, class V>
void HashTable<K, V>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    for (int i = 0; i < HashTableSize; i++)
        if (table[i])
            f((ManagedObject *&)table[i]);
}

template <class K, class V>
int HashTable<K, V>::getSize() {
    return sizeof *this;
}

template <class K, class V>
typename HashTable<K, V>::Entry *HashTable<K, V>::createEntry(const K &key, const V &value) {
    return new Entry(key, value);
}

template <>
typename HashTable<std::string, Object *>::Entry *HashTable<std::string, Object *>::createEntry(const std::string &key, Object *const &value) {
    Pointer<Object> p = value;

    Entry *entry = new Entry(key, 0);
    entry->setValue(p);

    return entry;
}
