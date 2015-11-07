#pragma once

#include <vector>

#include "common.h"

class Vector {
    static const int InitialCapacity = 128;

    int size, capacity;
    byte *data;

public:
    Vector();
    Vector(const Vector &v);
    Vector(const std::vector<byte> &v);

    ~Vector();

    byte *allocate(int count);
    bool free(int count);
    void release();

    bool enoughSpace(int count) const;

    byte *getData() const;
    int getSize() const;
    int getCapacity() const;
};
