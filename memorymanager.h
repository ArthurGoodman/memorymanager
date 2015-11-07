#pragma once

#include "vector.h"

class Object;

class MemoryManager {
    static MemoryManager *manager;

    Vector memory;

public:
    static MemoryManager *instance();

    Object *allocate(int size);
    void free(Object *object);

private:
    MemoryManager();
};
