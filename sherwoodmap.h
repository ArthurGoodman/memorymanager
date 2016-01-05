#pragma once

#include "map.h"

template <class K, class V>
class SherwoodMap : public Map<K, V> {
    class Entry : public Map<K, V>::Entry {
        uint hash;

    public:
        Entry();
        Entry(K key, V value);

        K &getKey();
        V &getValue();
        uint &getHash();

        bool equals(const K &key);

        void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
        int getSize() const;
    };

    static const int InitialCapacity = 4;
    static const int LoadFactorPercent = 90;

    Entry *buffer;
    int numElems, capacity, mask, resizeThreshold;

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
    void grow();
    void insertHelper(uint hash, K key, V value);
    static uint hashKey(const K &key);
    static bool isDeleted(uint hash);
    int desiredPos(uint hash) const;
    int probeDistance(uint hash, uint slotIndex) const;
    void construct(int index, uint hash, const K &key, const V &value);
    int lookupIndex(const K &key) const;
};
