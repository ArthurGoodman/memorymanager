#pragma once

#include "common.h"

class ByteArray {
    static int initialCapacity;

    int size, capacity;
    byte *data;

public:
    static void setInitialCapacity(int initialCapacity);

    ByteArray();
    ~ByteArray();

    byte *allocate(int count);
    bool free(int count);
    void release();

    bool enoughSpace(int count) const;

    byte *getData() const;
    int getSize() const;
    int getCapacity() const;
};
