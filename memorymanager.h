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
    static void shiftPointer(T *&pointer, int delta);

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);

private:
    MemoryManager();

    void shiftPointers();
};

template <typename T>
void MemoryManager::shiftPointer(T *&pointer, int delta) {
    pointer = (T *)((byte *)pointer + delta);
}

inline MemoryManager *MemoryManager::instance() {
    return manager;
}
