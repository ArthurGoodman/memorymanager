#include "sherwoodmap.h"

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
typename SherwoodMap<K, V>::iterator &SherwoodMap<K, V>::iterator::operator++() {
    while (++index < capacity && (buffer[index].getHash() == 0 || buffer[index].isDeleted())) {
    }

    return *this;
}

template <class K, class V>
typename SherwoodMap<K, V>::iterator &SherwoodMap<K, V>::iterator::operator*() {
    return *this;
}

template <class K, class V>
bool SherwoodMap<K, V>::iterator::operator!=(const iterator &other) const {
    return index != other.index;
}

template <class K, class V>
K &SherwoodMap<K, V>::iterator::key() {
    return buffer[index].getKey();
}

template <class K, class V>
V &SherwoodMap<K, V>::iterator::value() {
    return buffer[index].getValue();
}

template <class K, class V>
SherwoodMap<K, V>::iterator::iterator(SherwoodMap::Entry *buffer, int capacity, int index)
    : buffer(buffer), capacity(capacity), index(index) {
}

template <class K, class V>
SherwoodMap<K, V>::SherwoodMap()
    : buffer(0), numEntries(0), capacity(HalfInitialCapacity), resizeThreshold(0) {
}

template <class K, class V>
typename SherwoodMap<K, V>::iterator SherwoodMap<K, V>::begin() {
    if (numEntries == 0)
        return iterator(buffer, capacity, capacity);

    for (int i = 0; i < capacity; i++)
        if (buffer[i].getHash() && !buffer[i].isDeleted())
            return iterator(buffer, capacity, i);

    return iterator(buffer, capacity, capacity);
}

template <class K, class V>
typename SherwoodMap<K, V>::iterator SherwoodMap<K, V>::end() {
    return iterator(buffer, capacity, capacity);
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

    Pointer<SherwoodMap> _this = this;

    if (++numEntries >= resizeThreshold)
        allocate();

    _this->insert(hashKey(key), key, value);
}

template <>
void SherwoodMap<uint, Object *>::put(const uint &key, Object *const &value) {
    std::cout << "SherwoodMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    Pointer<SherwoodMap> _this = this;
    Pointer<Object> pValue = value;

    if (++numEntries >= resizeThreshold)
        allocate();

    _this->insert(hashKey(key), key, pValue);
}

template <>
void SherwoodMap<Object *, uint>::put(Object *const &key, const uint &value) {
    std::cout << "SherwoodMap<K, V>::put(key=" << key << ", value=" << value << ")\n";

    Pointer<SherwoodMap> _this = this;
    Pointer<Object> pKey = key;

    if (++numEntries >= resizeThreshold)
        allocate();

    _this->insert(hashKey(pKey), pKey, value);
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
int SherwoodMap<K, V>::size() const {
    return numEntries;
}

template <class K, class V>
void SherwoodMap<K, V>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    Object::mapOnReferences(f);

    if (buffer) {
        for (int i = 0; i < capacity; i++) {
            Entry *entry = buffer + i;
            f((ManagedObject *&)entry);
        }

        f((ManagedObject *&)buffer);
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
    std::cout << "SherwoodMap<K, V>::allocate() //capacity=" << capacity * 2 << "\n";

    Pointer<SherwoodMap> _this = this;
    Pointer<Entry> oldEntries = buffer;
    int oldCapacity = buffer ? capacity : 0;

    Entry *newBuffer = MemoryManager::instance()->allocateArray<Entry>(capacity * 2);
    _this->buffer = newBuffer;
    _this->capacity *= 2;
    _this->resizeThreshold = (_this->capacity * LoadFactorPercent) / 100;
    _this->mask = _this->capacity - 1;

    for (int i = 0; i < oldCapacity; i++) {
        Entry &entry = (*oldEntries)[i];

        if (entry.getHash() && !entry.isDeleted())
            _this->insert(entry.getHash(), entry.getKey(), entry.getValue());
    }
}

template <class K, class V>
void SherwoodMap<K, V>::insert(uint hash, K key, V value) {
    int pos = hash & mask;
    int dist = 0;

    while (true) {
        if (buffer[pos].getHash() == 0) {
            createEntry(pos, hash, key, value);
            return;
        }

        if (buffer[pos].getHash() == hash && buffer[pos].equals(key)) {
            createEntry(pos, hash, key, value);
            numEntries--;
            return;
        }

        int existingEntryProbeDist = probeDistance(buffer[pos].getHash(), pos);

        if (dist > existingEntryProbeDist) {
            if (buffer[pos].isDeleted()) {
                createEntry(pos, hash, key, value);
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
void SherwoodMap<K, V>::createEntry(int index, uint hash, const K &key, const V &value) {
    new (buffer + index) Entry(key, value);
    buffer[index].getHash() = hash;
}

template <class K, class V>
int SherwoodMap<K, V>::lookup(const K &key) const {
    const uint hash = hashKey(key);

    int pos = hash & mask;
    int dist = 0;

    while (true) {
        if (buffer == 0 || buffer[pos].getHash() == 0)
            return -1;
        else if (dist > probeDistance(buffer[pos].getHash(), pos))
            return -1;
        else if (buffer[pos].getHash() == hash && buffer[pos].equals(key))
            return pos;

        pos = (pos + 1) & mask;
        dist++;
    }
}
