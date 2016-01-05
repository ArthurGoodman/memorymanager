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
void SherwoodMap<K, V>::Entry::clear() {
    hash |= deletedFlag();
}

template <>
void SherwoodMap<uint, Object *>::Entry::clear() {
    hash |= deletedFlag();
    value = 0;
}

template <>
void SherwoodMap<Object *, uint>::Entry::clear() {
    hash |= deletedFlag();
    key = 0;
}

template <class K, class V>
bool SherwoodMap<K, V>::Entry::equals(const K &key) {
    return this->key == key;
}

template <>
bool SherwoodMap<Object *, uint>::Entry::equals(Object *const &key) {
    return this->key->equals(key);
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
    : buffer(0), numEntries(0), capacity(HalfOfInitialCapacity), resizeThreshold(0) {
}

template <class K, class V>
V SherwoodMap<K, V>::get(const K &key) const {
    std::cout << "SherwoodMap<K, V>::get(key=" << key << ")\n";

    const int index = lookup(key);

    if (index == -1)
        throw std::out_of_range("SherwoodMap<K, V>::get");

    return buffer[index].getValue();
}

template <class K, class V>
void SherwoodMap<K, V>::put(const K &key, const V &value) {
    std::cout << "SherwoodMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    Pointer<Map<K, V>> _this = this;

    if (++numEntries >= resizeThreshold)
        allocate();

    ((SherwoodMap *)*_this)->insert(hashKey(key), std::move(const_cast<K &>(key)), std::move(const_cast<V &>(value)));
}

template <>
void SherwoodMap<uint, Object *>::put(const uint &key, Object *const &value) {
    std::cout << "SherwoodMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    Pointer<Map<uint, Object *>> _this = this;
    Pointer<Object> pValue = value;

    if (++numEntries >= resizeThreshold)
        allocate();

    ((SherwoodMap *)*_this)->insert(hashKey(key), std::move(const_cast<uint &>(key)), std::move(const_cast<Object *&>(*pValue)));
}

template <>
void SherwoodMap<Object *, uint>::put(Object *const &key, const uint &value) {
    std::cout << "SherwoodMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    Pointer<Map<Object *, uint>> _this = this;
    Pointer<Object> pKey = key;

    if (++numEntries >= resizeThreshold)
        allocate();

    ((SherwoodMap *)*_this)->insert(hashKey(pKey), std::move(const_cast<Object *&>(*pKey)), std::move(const_cast<uint &>(value)));
}

template <class K, class V>
bool SherwoodMap<K, V>::remove(const K &key) {
    std::cout << "SherwoodMap<K, V>::remove(key=" << key << ")\n";

    const int index = lookup(key);

    if (index == -1)
        return false;

    buffer[index].clear();
    numEntries--;

    return true;
}

template <class K, class V>
bool SherwoodMap<K, V>::contains(const K &key) const {
    std::cout << "SherwoodMap<K, V>::contains(key=" << key << ")\n";

    return lookup(key) != -1;
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
uint SherwoodMap<K, V>::computeHash(const K &key) {
    return std::hash<K>()(key);
}

template <>
uint SherwoodMap<Object *, uint>::computeHash(Object *const &key) {
    return key->hash();
}

template <class K, class V>
uint SherwoodMap<K, V>::hashKey(const K &key) {
    uint hash = computeHash(key);

    hash &= ~Entry::deletedFlag();
    hash |= hash == 0;

    return hash;
}

template <class K, class V>
void SherwoodMap<K, V>::allocate() {
    std::cout << "SherwoodMap<K, V>::allocate() //capacity=" << capacity << "\n";

    Pointer<Map<K, V>> _this = this;
    Pointer<typename Map<K, V>::Entry> oldEntries = this->buffer;
    int oldCapacity = this->buffer ? capacity : 0;

    Entry *buffer = MemoryManager::instance()->allocateArray<Entry>(capacity * 2);
    ((SherwoodMap *)*_this)->buffer = buffer;
    ((SherwoodMap *)*_this)->capacity *= 2;

    ((SherwoodMap *)*_this)->resizeThreshold = (((SherwoodMap *)*_this)->capacity * LoadFactorPercent) / 100;

    for (int i = 0; i < oldCapacity; i++) {
        Entry &e = ((Entry *)*oldEntries)[i];

        uint hash = e.getHash();

        if (hash && !(hash & Entry::deletedFlag()))
            ((SherwoodMap *)*_this)->insert(hash, std::move(e.getKey()), std::move(e.getValue()));
    }
}

template <class K, class V>
void SherwoodMap<K, V>::insert(uint hash, K &&key, V &&value) {
    int pos = hash % capacity;
    int dist = 0;

    while (true) {
        if (!buffer[pos].getHash()) {
            createEntry(pos, hash, key, value);
            return;
        }

        int existingEntryProbeDist = probeDistance(buffer[pos].getHash(), pos);

        if (existingEntryProbeDist < dist) {
            if (buffer[pos].getHash() & Entry::deletedFlag()) {
                createEntry(pos, hash, key, value);
                return;
            }

            std::swap(key, buffer[pos].getKey());
            std::swap(value, buffer[pos].getValue());
            std::swap(hash, buffer[pos].getHash());

            dist = existingEntryProbeDist;
        }

        pos = (pos + 1) % capacity;
        dist++;
    }
}

template <class K, class V>
void SherwoodMap<K, V>::createEntry(int index, uint hash, const K &key, const V &value) {
    new (buffer + index) Entry(key, value);
    buffer[index].getHash() = hash;
}

template <class K, class V>
int SherwoodMap<K, V>::lookup(const K &key) const {
    const uint hash = hashKey(key);

    int pos = hash % capacity;
    int dist = 0;

    while (true) {
        if (!buffer || !buffer[pos].getHash())
            return -1;
        else if (dist > probeDistance(buffer[pos].getHash(), pos))
            return -1;
        else if (buffer[pos].getHash() == hash && buffer[pos].equals(key))
            return pos;

        pos = (pos + 1) % capacity;
        dist++;
    }
}
