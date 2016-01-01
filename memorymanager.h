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

    Pointer<ManagedObject> *firstPointer;

public:
    static MemoryManager *instance();

    template <typename T>
    static void shiftPointer(T *&pointer, int delta);

    ManagedObject *allocate(int size);

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);

    int getDelta();

private:
    MemoryManager();

    void shiftPointers();

    void collectGarbage();
    void mark();
    void sweep();
    void defragment();
};

template <typename T>
void MemoryManager::shiftPointer(T *&pointer, int delta) {
    pointer = (T *)((byte *)pointer + delta);
}
