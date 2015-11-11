#pragma once

#include "vector.h"

class Object;

class MemoryManager {
    static MemoryManager *manager;

    Vector memory;
    int delta;

    int objectCount;

public:
    static MemoryManager *instance();

    Object *allocate(int size);
    void free(Object *object);

    int getDelta();

private:
    MemoryManager();

    void shiftPointers();
};

inline MemoryManager *MemoryManager::instance() {
    return manager;
}
