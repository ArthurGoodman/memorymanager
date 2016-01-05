#pragma once

#include "map.h"

template <class K, class V>
class SherwoodMap : public Map<K, V> {
    class Entry : public Map<K, V>::Entry {
        uint hash;

    public:
        Entry();

        uint &getHash();

        void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
        int getSize() const;
    };

    static const int InitialCapacity = 11;
    static const int LoadFactorPercent = 90;

    Entry *buffer;
    uint numElems, capacity, mask, resizeThreshold;

public:
    SherwoodMap();

    V get(const K &key) const;
    void put(const K &key, const V &value);
    bool remove(const K &key);
    bool contains(const K &key) const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;

private:
    void allocate();
};
