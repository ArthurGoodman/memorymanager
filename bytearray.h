#pragma once

#include "common.h"

class ByteArray {
    static uint initialCapacity;

    uint size, capacity;
    int delta;
    byte *data;

public:
    static void setInitialCapacity(uint initialCapacity);

    ByteArray();
    ~ByteArray();

    byte *allocate(uint count);
    bool free(uint count);
    void release();

    bool enoughSpace(uint count) const;

    byte *getData() const;
    uint getSize() const;
    uint getCapacity() const;
    int getDelta() const;
};
