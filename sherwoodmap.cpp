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
V SherwoodMap<K, V>::get(const K & /*key*/) const {
    return (V)0;
}

template <class K, class V>
void SherwoodMap<K, V>::put(const K & /*key*/, const V & /*value*/) {
    if (!buffer)
        allocate();
}

template <class K, class V>
bool SherwoodMap<K, V>::remove(const K & /*key*/) {
    return false;
}

template <class K, class V>
bool SherwoodMap<K, V>::contains(const K & /*key*/) const {
    return false;
}

template <class K, class V>
void SherwoodMap<K, V>::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (buffer) {
        f((ManagedObject *&)buffer);

        for (uint i = 1; i < capacity; i++) {
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
