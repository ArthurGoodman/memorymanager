#pragma once

#include "vector.h"

class ManagedObject;

template <class>
class Pointer;

class MemoryManager {
    static MemoryManager manager;

    Vector memory;
    int delta;

    int objectCount;

    Pointer<ManagedObject> *pointers;

public:
    static MemoryManager *instance();

    template <typename T>
    static void shiftPointer(T *&pointer);

    ManagedObject *allocate(int size);

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);

private:
    MemoryManager();

    void shiftPointers();

    void collectGarbage();
    void mark();
    void compact();
};

template <typename T>
void MemoryManager::shiftPointer(T *&pointer) {
    pointer = (T *)((byte *)pointer + manager.delta);
}
