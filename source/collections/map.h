#pragma once

#include "managedobject.h"

template <class K, class V>
class Map : public ManagedObject {
public:
    virtual V &get(const K &key) const = 0;
    virtual V &put(const K &key, const V &value) = 0;
    virtual bool remove(const K &key) = 0;
    virtual bool contains(const K &key) const = 0;

    virtual int size() const = 0;

    V &operator[](const K &key);
};

template <class K, class V>
V &Map<K, V>::operator[](const K &key) {
    if (!contains(key))
        return put(key, V());

    return get(key);
}
