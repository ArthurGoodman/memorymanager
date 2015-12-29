#pragma once

#include "vector.h"

class ManagedObject;

template <typename>
class Pointer;

class MemoryManager {
    static MemoryManager *manager;

    Vector memory;
    int delta;

    int objectCount;

    Pointer<ManagedObject> *firstPointer;

public:
    static MemoryManager *instance();

    ManagedObject *allocate(int size);
    void free(ManagedObject *object);

    template <typename T>
    static T *shiftPointer(T *pointer);

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);

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
