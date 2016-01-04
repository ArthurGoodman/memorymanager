#include "chainmap.h"

#include <stdexcept>

#include "pointer.h"

#include <iostream>

template class ChainMap<uint, Object *>;
template class ChainMap<Object *, uint>;

template <class K, class V>
ChainMap<K, V>::Entry::Entry(const K &key, const V &value)
    : HashMap<K, V>::Entry(key, value), next(0) {
}

template <class K, class V>
K &ChainMap<K, V>::Entry::getKey() {
    return this->key;
}

template <class K, class V>
V &ChainMap<K, V>::Entry::getValue() {
    return this->value;
}

template <class K, class V>
void ChainMap<K, V>::Entry::setKey(const K &key) {
    this->key = key;
}

template <class K, class V>
void ChainMap<K, V>::Entry::setValue(const V &value) {
    this->value = value;
}

template <class K, class V>
typename ChainMap<K, V>::Entry *ChainMap<K, V>::Entry::getNext() const {
    return next;
}

template <class K, class V>
void ChainMap<K, V>::Entry::setNext(ChainMap<K, V>::Entry *next) {
    this->next = next;
}

template <class K, class V>
bool ChainMap<K, V>::Entry::equals(const K &key) const {
    return this->key == key;
}

template <>
bool ChainMap<Object *, uint>::Entry::equals(Object *const &key) const {
    return this->key->equals(key);
}

template <class K, class V>
void ChainMap<K, V>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    if (next)
        f((ManagedObject *&)next);
}

template <>
void ChainMap<uint, Object *>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    if (next)
        f((ManagedObject *&)next);

    if (value)
        f((ManagedObject *&)value);
}

template <>
void ChainMap<Object *, uint>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    if (next)
        f((ManagedObject *&)next);

    if (key)
        f((ManagedObject *&)key);
}

template <class K, class V>
int ChainMap<K, V>::Entry::getSize() {
    return sizeof *this;
}

template <class K, class V>
typename ChainMap<K, V>::iterator &ChainMap<K, V>::iterator::operator++() {
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
typename ChainMap<K, V>::iterator &ChainMap<K, V>::iterator::operator*() {
    return *this;
}

template <class K, class V>
bool ChainMap<K, V>::iterator::operator!=(const iterator &other) const {
    return entry != ((const ChainMap::iterator &)other).entry;
}

template <class K, class V>
K &ChainMap<K, V>::iterator::key() {
    return entry->getKey();
}

template <class K, class V>
V &ChainMap<K, V>::iterator::value() {
    return entry->getValue();
}

template <class K, class V>
ChainMap<K, V>::iterator::iterator(ChainMap<K, V>::Entry **table)
    : table(table), i(0), entry(0) {
}

template <class K, class V>
ChainMap<K, V>::iterator::iterator(ChainMap<K, V>::Entry **table, int i)
    : table(table), i(i), entry(table[i]) {
}

template <class K, class V>
ChainMap<K, V>::ChainMap()
    : table{0} {
}

template <class K, class V>
typename ChainMap<K, V>::iterator ChainMap<K, V>::begin() {
    for (int i = 0; i < TableSize; i++)
        if (table[i])
            return iterator(table, i);

    return iterator(table);
}

template <class K, class V>
typename ChainMap<K, V>::iterator ChainMap<K, V>::end() {
    return iterator(table);
}

template <class K, class V>
V ChainMap<K, V>::get(const K &key) const {
    std::cout << "ChainMap<K, V>::get(key=" << key << ")\n";

    ulong hashValue = hash(key) % TableSize;
    Entry *entry = table[hashValue];

    while (entry) {
        if (entry->equals(key))
            return entry->getValue();

        entry = entry->getNext();
    }

    throw std::out_of_range("ChainMap<K, V>::get");
}

template <class K, class V>
void ChainMap<K, V>::put(const K &key, const V &value) {
    std::cout << "ChainMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    ulong hashValue = hash(key) % TableSize;

    Pointer<typename HashMap<K, V>::Entry> prev;
    Pointer<typename HashMap<K, V>::Entry> entry = table[hashValue];

    while (entry && !((Entry *)*entry)->equals(key)) {
        prev = entry;
        entry = ((Entry *)*entry)->getNext();
    }

    if (entry) {
        ((Entry *)*entry)->setValue(value);
        return;
    }

    Pointer<HashMap<K, V>> _this = (HashMap<K, V> *)this;

    entry = createEntry(key, value);

    if (!prev)
        ((ChainMap *)*_this)->table[hashValue] = (Entry *)*entry;
    else
        ((Entry *)*prev)->setNext((Entry *)*entry);
}

template <class K, class V>
bool ChainMap<K, V>::remove(const K &key) {
    std::cout << "ChainMap<K, V>::remove(key=" << key << ")\n";

    ulong hashValue = hash(key) % TableSize;

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
bool ChainMap<K, V>::contains(const K &key) const {
    std::cout << "ChainMap<K, V>::contains(key=" << key << ")\n";

    ulong hashValue = hash(key) % TableSize;

    Entry *entry = table[hashValue];

    while (entry && !entry->equals(key))
        entry = entry->getNext();

    return entry;
}

template <class K, class V>
void ChainMap<K, V>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    for (int i = 0; i < TableSize; i++)
        if (table[i])
            f((ManagedObject *&)table[i]);
}

template <class K, class V>
int ChainMap<K, V>::getSize() {
    return sizeof *this;
}

template <class K, class V>
typename ChainMap<K, V>::Entry *ChainMap<K, V>::createEntry(const K &key, const V &value) const {
    return new Entry(key, value);
}

template <>
typename ChainMap<uint, Object *>::Entry *ChainMap<uint, Object *>::createEntry(const uint &key, Object *const &value) const {
    Pointer<Object> p = value;

    Entry *entry = new Entry(key, 0);
    entry->setValue(p);

    return entry;
}

template <>
typename ChainMap<Object *, uint>::Entry *ChainMap<Object *, uint>::createEntry(Object *const &key, const uint &value) const {
    Pointer<Object> p = key;

    Entry *entry = new Entry(0, value);
    entry->setKey(p);

    return entry;
}

template <class K, class V>
ulong ChainMap<K, V>::hash(const K &key) const {
    return hashFunction(key);
}

template <>
ulong ChainMap<Object *, uint>::hash(Object *const &key) const {
    return key->hash();
}
