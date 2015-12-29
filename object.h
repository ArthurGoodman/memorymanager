#pragma once

#include "common.h"

#include "managedobject.h"

//template <typename K, typename V, typename F = std::hash<K>>
//class HashTable;

class Object : public ManagedObject {
    //    HashTable<char *, Object *> *attributes;

public:
    Object();

    virtual void mark();

    virtual int getSize();
};
