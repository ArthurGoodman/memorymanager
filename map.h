#pragma once

#include "object.h"

template <class K, class V>
class Map : public Object {
public:
    virtual V get(const K &key) const = 0;
    virtual void put(const K &key, const V &value) = 0;
    virtual bool remove(const K &key) = 0;
    virtual bool contains(const K &key) const = 0;

    virtual int size() const = 0;
};
