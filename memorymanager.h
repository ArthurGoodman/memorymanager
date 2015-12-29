#pragma once

#include "vector.h"

class Object;
class Pointer;

class MemoryManager {
    static MemoryManager *manager;

    Vector memory;
    int delta;

    int objectCount;

    Pointer *firstPointer;

public:
    static MemoryManager *instance();

    Object *allocate(int size);
    void free(Object *object);

    template <typename T>
    static T *shiftPointer(T *pointer);

    void registerPointer(Pointer *pointer);
    void removePointer(Pointer *pointer);

private:
    MemoryManager();

    void shiftPointers();
};

template <typename T>
T *MemoryManager::shiftPointer(T *pointer) {
    return (T *)((byte *)pointer + MemoryManager::instance()->delta);
}

inline MemoryManager *MemoryManager::instance() {
    return manager;
}
