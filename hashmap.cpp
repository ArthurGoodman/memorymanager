#include "hashmap.h"

#include <stdexcept>

#include "pointer.h"

#include <iostream>

template class HashMap<uint, Object *>;
template class HashMap<Object *, uint>;

template <class K, class V>
HashMap<K, V>::Entry::Entry(const K &key, const V &value)
    : Map<K, V>::Entry(key, value), next(0) {
}

template <class K, class V>
K &HashMap<K, V>::Entry::getKey() {
    return this->key;
}

template <class K, class V>
V &HashMap<K, V>::Entry::getValue() {
    return this->value;
}

template <class K, class V>
void HashMap<K, V>::Entry::setKey(const K &key) {
    this->key = key;
}

template <class K, class V>
void HashMap<K, V>::Entry::setValue(const V &value) {
    this->value = value;
}

template <class K, class V>
typename HashMap<K, V>::Entry *HashMap<K, V>::Entry::getNext() const {
    return next;
}

template <class K, class V>
void HashMap<K, V>::Entry::setNext(HashMap<K, V>::Entry *next) {
    this->next = next;
}

template <class K, class V>
bool HashMap<K, V>::Entry::equals(const K &key) const {
    return this->key == key;
}

template <>
bool HashMap<Object *, uint>::Entry::equals(Object *const &key) const {
    return this->key->equals(key);
}

template <class K, class V>
void HashMap<K, V>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (next)
        f((ManagedObject *&)next);
}

template <>
void HashMap<uint, Object *>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (next)
        f((ManagedObject *&)next);

    if (value)
        f((ManagedObject *&)value);
}

template <>
void HashMap<Object *, uint>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (next)
        f((ManagedObject *&)next);

    if (key)
        f((ManagedObject *&)key);
}

template <class K, class V>
int HashMap<K, V>::Entry::getSize() const {
    return sizeof *this;
}

template <class K, class V>
typename HashMap<K, V>::iterator &HashMap<K, V>::iterator::operator++() {
    if (entry->getNext())
        entry = entry->getNext();
    else {
        while (++i < TableSize)
            if (table[i]) {
                entry = table[i];
                break;
            }

        if (i == TableSize)
            entry = 0;
    }

    return *this;
}

template <class K, class V>
typename HashMap<K, V>::iterator &HashMap<K, V>::iterator::operator*() {
    return *this;
}

template <class K, class V>
bool HashMap<K, V>::iterator::operator!=(const iterator &other) const {
    return entry != ((const HashMap::iterator &)other).entry;
}

template <class K, class V>
K &HashMap<K, V>::iterator::key() {
    return entry->getKey();
}

template <class K, class V>
V &HashMap<K, V>::iterator::value() {
    return entry->getValue();
}

template <class K, class V>
HashMap<K, V>::iterator::iterator()
    : table(0), entry(0), i(0) {
}

template <class K, class V>
HashMap<K, V>::iterator::iterator(HashMap<K, V>::Entry **table, int i)
    : table(table), entry(table[i]), i(i) {
}

template <class K, class V>
HashMap<K, V>::HashMap()
    : table{0} {
}

template <class K, class V>
typename HashMap<K, V>::iterator HashMap<K, V>::begin() {
    for (int i = 0; i < TableSize; i++)
        if (table[i])
            return iterator(table, i);

    return iterator();
}

template <class K, class V>
typename HashMap<K, V>::iterator HashMap<K, V>::end() {
    return iterator();
}

template <class K, class V>
V HashMap<K, V>::get(const K &key) const {
    std::cout << "HashMap<K, V>::get(key=" << key << ")\n";

    ulong hashValue = hashKey(key) % TableSize;
    Entry *entry = table[hashValue];

    while (entry) {
        if (entry->equals(key))
            return entry->getValue();

        entry = entry->getNext();
    }

    throw std::out_of_range("HashMap<K, V>::get");
}

template <class K, class V>
void HashMap<K, V>::put(const K &key, const V &value) {
    std::cout << "HashMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    ulong hashValue = hashKey(key) % TableSize;

    Pointer<Entry> prev;
    Pointer<Entry> entry = table[hashValue];

    while (entry && !((Entry *)*entry)->equals(key)) {
        prev = entry;
        entry = ((Entry *)*entry)->getNext();
    }

    if (entry) {
        ((Entry *)*entry)->setValue(value);
        return;
    }

    Pointer<HashMap> _this = this;

    entry = createEntry(key, value);

    if (!prev)
        _this->table[hashValue] = entry;
    else
        prev->setNext(entry);
}

template <class K, class V>
bool HashMap<K, V>::remove(const K &key) {
    std::cout << "HashMap<K, V>::remove(key=" << key << ")\n";

    ulong hashValue = hashKey(key) % TableSize;

    Entry *prev = 0;
    Entry *entry = table[hashValue];

    while (entry && !entry->equals(key)) {
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
bool HashMap<K, V>::contains(const K &key) const {
    std::cout << "HashMap<K, V>::contains(key=" << key << ")\n";

    ulong hashValue = hashKey(key) % TableSize;

    Entry *entry = table[hashValue];

    while (entry && !entry->equals(key))
        entry = entry->getNext();

    return entry;
}

template <class K, class V>
void HashMap<K, V>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    for (int i = 0; i < TableSize; i++)
        if (table[i])
            f((ManagedObject *&)table[i]);
}

template <class K, class V>
int HashMap<K, V>::getSize() const {
    return sizeof *this;
}

template <class K, class V>
ulong HashMap<K, V>::hashKey(const K &key) {
    return std::hash<K>()(key);
}

template <>
ulong HashMap<Object *, uint>::hashKey(Object *const &key) {
    return key->hash();
}

template <class K, class V>
typename HashMap<K, V>::Entry *HashMap<K, V>::createEntry(const K &key, const V &value) const {
    return new Entry(key, value);
}

template <>
typename HashMap<uint, Object *>::Entry *HashMap<uint, Object *>::createEntry(const uint &key, Object *const &value) const {
    Pointer<Object> p = value;

    Entry *entry = new Entry(key, 0);
    entry->setValue(p);

    return entry;
}

template <>
typename HashMap<Object *, uint>::Entry *HashMap<Object *, uint>::createEntry(Object *const &key, const uint &value) const {
    Pointer<Object> p = key;

    Entry *entry = new Entry(0, value);
    entry->setKey(p);

    return entry;
}
