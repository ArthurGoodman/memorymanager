#pragma once

#include "map.h"

template <class K, class V>
class SherwoodMap : public Map<K, V> {
    class Entry : public Map<K, V>::Entry {
    };

public:
    V get(const K &key) const;
    void put(const K &key, const V &value);
    bool remove(const K &key);
    bool contains(const K &key) const;
};
