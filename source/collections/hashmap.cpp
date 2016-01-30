#include "hashmap.h"

#include <stdexcept>

#include "object.h"
#include "array.h"

#include <iostream>

template class HashMap<uint, Object *>;
template class HashMap<Object *, uint>;

template <class K, class V>
HashMap<K, V>::Entry::Entry(const K &key, const V &value)
    : key(key), value(value), next(0) {
}

template <class K, class V>
K &HashMap<K, V>::Entry::getKey() {
    return key;
}

template <class K, class V>
V &HashMap<K, V>::Entry::getValue() {
    return value;
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
void HashMap<K, V>::Entry::setNext(Entry *next) {
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
        while (++i < capacity)
            if ((*buffer)[i]) {
                entry = (*buffer)[i];
                break;
            }

        if (i == capacity)
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
    : buffer(0), entry(0), capacity(0), i(0) {
}

template <class K, class V>
HashMap<K, V>::iterator::iterator(Array<Entry *> *buffer, int capacity, int i)
    : buffer(buffer), entry((*buffer)[i]), capacity(capacity), i(i) {
}

template <class K, class V>
HashMap<K, V>::HashMap()
    : buffer(0), numEntries(0), capacity(HalfInitialCapacity), resizeThreshold(0) {
}

template <class K, class V>
typename HashMap<K, V>::iterator HashMap<K, V>::begin() {
    for (int i = 0; i < capacity; i++)
        if ((*buffer)[i])
            return iterator(buffer, capacity, i);

    return iterator();
}

template <class K, class V>
typename HashMap<K, V>::iterator HashMap<K, V>::end() {
    return iterator();
}

template <class K, class V>
V &HashMap<K, V>::get(const K &key) const {
    std::cout << "HashMap<K, V>::get(key=" << key << ")\n";

    Entry *entry = lookup(key);

    if (entry == 0)
        throw std::out_of_range("HashMap<K, V>::get");

    return entry->getValue();
}

template <class K, class V>
V &HashMap<K, V>::put(const K &key, const V &value) {
    std::cout << "HashMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    Pointer<HashMap> _this = this;

    if (++numEntries >= resizeThreshold)
        allocate();

    return _this->insert(key, value);
}

template <>
Object *&HashMap<uint, Object *>::put(const uint &key, Object *const &value) {
    std::cout << "HashMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    HashMap *_this = this;
    Object *pValue = value;

    GC_FRAME(POINTER(_this) POINTER(pValue))

    if (++numEntries >= resizeThreshold)
        allocate();

    return _this->insert(key, pValue);
}

template <>
uint &HashMap<Object *, uint>::put(Object *const &key, const uint &value) {
    std::cout << "HashMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    HashMap *_this = this;
    Object *pKey = key;

    GC_FRAME(POINTER(_this) POINTER(pKey))

    if (++numEntries >= resizeThreshold)
        allocate();

    return _this->insert(pKey, value);
}

template <class K, class V>
bool HashMap<K, V>::remove(const K &key) {
    std::cout << "HashMap<K, V>::remove(key=" << key << ")\n";

    if (buffer == 0)
        return false;

    int hashValue = hashKey(key) & mask;

    Entry *prev = 0;
    Entry *entry = (*buffer)[hashValue];

    while (entry && !entry->equals(key)) {
        prev = entry;
        entry = entry->getNext();
    }

    if (entry == 0)
        return false;

    if (prev == 0)
        (*buffer)[hashValue] = entry->getNext();
    else
        prev->setNext(entry->getNext());

    numEntries--;

    return true;
}

template <class K, class V>
bool HashMap<K, V>::contains(const K &key) const {
    std::cout << "HashMap<K, V>::contains(key=" << key << ")\n";

    return lookup(key) != 0;
}

template <class K, class V>
int HashMap<K, V>::size() const {
    return numEntries;
}

template <>
void Array<typename HashMap<uint, Object *>::Entry *>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    for (int i = 0; i < size(); i++)
        if (data()[i])
            f((ManagedObject *&)data()[i]);
}

template <>
void Array<typename HashMap<Object *, uint>::Entry *>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    for (int i = 0; i < size(); i++)
        if (data()[i])
            f((ManagedObject *&)data()[i]);
}

template <class K, class V>
void HashMap<K, V>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (buffer)
        f((ManagedObject *&)buffer);
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
void HashMap<K, V>::allocate() {
    std::cout << "HashMap<K, V>::allocate() //capacity=" << capacity * 2 << "\n";

    HashMap *_this = this;
    Array<Entry *> *oldEntries = buffer;

    GC_FRAME(POINTER(_this) POINTER(oldEntries))

    int oldCapacity = buffer ? capacity : 0;

    Array<Entry *> *newBuffer = Array<Entry *>::create(capacity * 2);
    _this->buffer = newBuffer;
    _this->capacity *= 2;
    _this->resizeThreshold = (_this->capacity * LoadFactorPercent) / 100;
    _this->mask = _this->capacity - 1;

    for (Entry *&entry : *_this->buffer)
        entry = 0;

    for (int i = 0; i < oldCapacity; i++) {
        Entry *prev = 0;
        Entry *entry = (**oldEntries)[i];

        while (entry) {
            uint hashValue = hashKey(entry->getKey()) & _this->mask;

            prev = entry;
            entry = entry->getNext();

            prev->setNext((*_this->buffer)[hashValue]);
            (*_this->buffer)[hashValue] = prev;
        }
    }
}

template <class K, class V>
V &HashMap<K, V>::insert(const K &key, const V &value) {
    int hashValue = hashKey(key) & mask;

    Entry *prev = 0;
    Entry *entry = (*buffer)[hashValue];
    HashMap *_this = this;

    GC_FRAME(POINTER(prev) POINTER(entry) POINTER(_this))

    while (entry && !entry->equals(key)) {
        prev = entry;
        entry = entry->getNext();
    }

    if (entry) {
        entry->setValue(value);
        numEntries--;
        return entry->getValue();
    }

    entry = createEntry(key, value);

    if (prev == 0)
        (*_this->buffer)[hashValue] = entry;
    else
        prev->setNext(entry);

    return entry->getValue();
}

template <class K, class V>
typename HashMap<K, V>::Entry *HashMap<K, V>::lookup(const K &key) const {
    if (buffer == 0)
        return 0;

    Entry *entry = (*buffer)[hashKey(key) & mask];

    while (entry && !entry->equals(key))
        entry = entry->getNext();

    return entry;
}

template <class K, class V>
typename HashMap<K, V>::Entry *HashMap<K, V>::createEntry(const K &key, const V &value) const {
    return new Entry(key, value);
}

template <>
typename HashMap<uint, Object *>::Entry *HashMap<uint, Object *>::createEntry(const uint &key, Object *const &value) const {
    Object *pValue = value;

    GC_FRAME(POINTER(pValue))

    Entry *entry = new Entry(key, 0);
    entry->setValue(pValue);

    return entry;
}

template <>
typename HashMap<Object *, uint>::Entry *HashMap<Object *, uint>::createEntry(Object *const &key, const uint &value) const {
    Object *pKey = key;

    GC_FRAME(POINTER(pKey))

    Entry *entry = new Entry(0, value);
    entry->setKey(pKey);

    return entry;
}
