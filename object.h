#pragma once

#include "common.h"

class HashTable;

class Object {
    int flags;
    HashTable *attributes;

public:
    enum Flag {
        FlagFree = 1 << 0,
        FlagMark = 1 << 1,
    };

public:
    Object();

    void setFlag(Flag flag, bool value = true);

    static void *operator new(uint size);
    static void operator delete(void *p);
};
