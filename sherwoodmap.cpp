#include "sherwoodmap.h"

#include "memorymanager.h"
#include "pointer.h"

#include <iostream>

template class SherwoodMap<uint, Object *>;
template class SherwoodMap<Object *, uint>;

template <class K, class V>
SherwoodMap<K, V>::Entry::Entry()
    : hash(0) {
}

template <>
SherwoodMap<uint, Object *>::Entry::Entry()
    : hash(0) {
    this->value = 0;
}

template <>
SherwoodMap<Object *, uint>::Entry::Entry()
    : hash(0) {
    this->key = 0;
}

template <class K, class V>
SherwoodMap<K, V>::Entry::Entry(K key, V value)
    : Map<K, V>::Entry(key, value) {
}

template <class K, class V>
K &SherwoodMap<K, V>::Entry::getKey() {
    return this->key;
}

template <class K, class V>
V &SherwoodMap<K, V>::Entry::getValue() {
    return this->value;
}

template <class K, class V>
uint &SherwoodMap<K, V>::Entry::getHash() {
    return hash;
}

template <class K, class V>
void SherwoodMap<K, V>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
}

template <>
void SherwoodMap<uint, Object *>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (value)
        f((ManagedObject *&)value);
}

template <>
void SherwoodMap<Object *, uint>::Entry::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (key)
        f((ManagedObject *&)key);
}

template <class K, class V>
int SherwoodMap<K, V>::Entry::getSize() const {
    return sizeof *this;
}

template <class K, class V>
SherwoodMap<K, V>::SherwoodMap()
    : buffer(0), numElems(0), capacity(InitialCapacity) {
}

template <class K, class V>
V SherwoodMap<K, V>::get(const K &key) const {
    std::cout << "SherwoodMap<K, V>::get(key=" << key << ")\n";

    const int index = lookupIndex(key);

    if (index == -1)
        throw std::out_of_range("SherwoodMap<K, V>::get");

    return buffer[index].getValue();
}

template <class K, class V>
void SherwoodMap<K, V>::put(const K &key, const V &value) {
    std::cout << "SherwoodMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    Pointer<Map<K, V>> _this = this;

    if (!buffer)
        allocate();

    if (++numElems >= resizeThreshold)
        ((SherwoodMap *)*_this)->grow();

    ((SherwoodMap *)*_this)->insertHelper(hashKey(key), key, value);
}

template <class K, class V>
bool SherwoodMap<K, V>::remove(const K &key) {
    std::cout << "SherwoodMap<K, V>::remove(key=" << key << ")\n";

    const int index = lookupIndex(key);

    if (index == -1)
        return false;

    buffer[index].getHash() |= 0x80000000;
    numElems--;

    return true;
}

template <class K, class V>
bool SherwoodMap<K, V>::contains(const K &key) const {
    std::cout << "SherwoodMap<K, V>::contains(key=" << key << ")\n";

    return lookupIndex(key) != -1;
}

template <class K, class V>
void SherwoodMap<K, V>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (buffer) {
        f((ManagedObject *&)buffer);

        for (int i = 1; i < capacity; i++) {
            Entry *entry = buffer + i;
            f((ManagedObject *&)entry);
        }
    }
}

template <class K, class V>
int SherwoodMap<K, V>::getSize() const {
    return sizeof *this;
}

template <class K, class V>
void SherwoodMap<K, V>::allocate() {
    Pointer<Map<K, V>> _this = this;

    Entry *buffer = MemoryManager::instance()->allocateArray<Entry>(capacity);
    ((SherwoodMap *)*_this)->buffer = buffer;

    ((SherwoodMap *)*_this)->resizeThreshold = (((SherwoodMap *)*_this)->capacity * LoadFactorPercent) / 100;
    ((SherwoodMap *)*_this)->mask = ((SherwoodMap *)*_this)->capacity - 1;
}

template <class K, class V>
void SherwoodMap<K, V>::grow() {
    std::cout << "SherwoodMap<K, V>::grow() //capacity=" << capacity << "\n";

    Pointer<typename Map<K, V>::Entry> oldEntries = buffer;
    int oldCapacity = capacity;

    capacity *= 2;

    Pointer<Map<K, V>> _this = this;

    allocate();

    for (int i = 0; i < oldCapacity; i++) {
        Entry &e = ((Entry *)*oldEntries)[i];

        uint hash = e.getHash();

        if (hash && !isDeleted(hash))
            ((SherwoodMap *)*_this)->insertHelper(hash, e.getKey(), e.getValue());
    }
}

template <class K, class V>
void SherwoodMap<K, V>::insertHelper(uint hash, K key, V value) {
    int pos = desiredPos(hash);
    int dist = 0;

    while (true) {
        if (buffer[pos].getHash() == 0) {
            construct(pos, hash, key, value);
            return;
        }

        int existingEntryProbeDist = probeDistance(buffer[pos].getHash(), pos);
        if (existingEntryProbeDist < dist) {
            if (isDeleted(buffer[pos].getHash())) {
                construct(pos, hash, key, value);
                return;
            }

            std::swap(key, buffer[pos].getKey());
            std::swap(value, buffer[pos].getValue());
            std::swap(hash, buffer[pos].getHash());

            dist = existingEntryProbeDist;
        }

        pos = (pos + 1) & mask;
        dist++;
    }
}

template <class K, class V>
uint SherwoodMap<K, V>::hashKey(const K &key) {
    uint h = static_cast<uint>(std::hash<K>()(key));

    h &= 0x7fffffff;
    h |= h == 0;

    return h;
}

template <>
uint SherwoodMap<Object *, uint>::hashKey(Object *const &key) {
    uint h = key->hash();

    h &= 0x7fffffff;
    h |= h == 0;

    return h;
}

template <class K, class V>
bool SherwoodMap<K, V>::isDeleted(uint hash) {
    return (hash >> 31) != 0;
}

template <class K, class V>
int SherwoodMap<K, V>::desiredPos(uint hash) const {
    return hash & mask;
}

template <class K, class V>
int SherwoodMap<K, V>::probeDistance(uint hash, uint slotIndex) const {
    return (slotIndex + capacity - desiredPos(hash)) & mask;
}

template <class K, class V>
void SherwoodMap<K, V>::construct(int index, uint hash, K key, V value) {
    new (buffer + index) Entry;

    buffer[index].getKey() = key;
    buffer[index].getValue() = value;
    buffer[index].getHash() = hash;
}

template <class K, class V>
int SherwoodMap<K, V>::lookupIndex(const K &key) const {
    const uint hash = hashKey(key);

    int pos = desiredPos(hash);
    int dist = 0;

    while (true) {
        if (!buffer || !buffer[pos].getHash())
            return -1;
        else if (dist > probeDistance(buffer[pos].getHash(), pos))
            return -1;
        else if (buffer[pos].getHash() == hash && buffer[pos].getKey() == key)
            return pos;

        pos = (pos + 1) & mask;
        dist++;
    }
}
