#pragma once

#include "common.h"

#include <functional>

template <typename K, typename V, typename F = std::hash<K>>
class HashTable;

class Object {
    int flags;
    int size;
    HashTable<char *, Object *> *attributes;

public:
    enum Flag {
        FlagFree = 1 << 0,
        FlagMark = 1 << 1,
    };

    static void *operator new(uint size);
    static void operator delete(void *p);

    Object();
    virtual ~Object();

    void setFlag(Flag flag, bool value = true);

    virtual void mark();

    virtual ulong hash();

    virtual void shiftPointers();
    virtual int getSize();
};
