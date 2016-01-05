#pragma once

#include "map.h"

template <class K, class V>
class SherwoodMap : public Map<K, V> {
    class Entry : public Map<K, V>::Entry {
        uint hash;

    public:
        static uint deletedFlag();

        Entry();
        Entry(K key, V value);

        K &getKey();
        V &getValue();
        uint &getHash();

        void clear();

        bool equals(const K &key);

        void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
        int getSize() const;
    };

    static const int HalfOfInitialCapacity = 4;
    static const int LoadFactorPercent = 90;

    Entry *buffer;
    int numEntries, capacity, resizeThreshold;

public:
    SherwoodMap();

    V get(const K &key) const;
    void put(const K &key, const V &value);
    bool remove(const K &key);
    bool contains(const K &key) const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;

private:
    static uint computeHash(const K &key);
    static uint hashKey(const K &key);

    void allocate();
    int probeDistance(uint hash, uint index) const;
    void insert(uint hash, K &&key, V &&value);
    void createEntry(int index, uint hash, const K &key, const V &value);
    int lookup(const K &key) const;
};

template <class K, class V>
inline uint SherwoodMap<K, V>::Entry::deletedFlag() {
    return 1 << (sizeof(hash) * 8 - 1);
}

template <class K, class V>
inline int SherwoodMap<K, V>::probeDistance(uint hash, uint index) const {
    return (index + capacity - hash % capacity) % capacity;
}
