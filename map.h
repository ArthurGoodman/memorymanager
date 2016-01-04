#pragma once

#include "object.h"

template <class K, class V>
class Map : public Object {
protected:
    class Entry : public Object {
    protected:
        K key;
        V value;

    public:
        Entry(const K &key, const V &value);
    };

public:
    virtual V get(const K &key) const = 0;
    virtual void put(const K &key, const V &value) = 0;
    virtual bool remove(const K &key) = 0;
    virtual bool contains(const K &key) const = 0;
};

template <class K, class V>
Map<K, V>::Entry::Entry(const K &key, const V &value)
    : key(key), value(value) {
}
