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
    static MemoryManager *instance() {
        return &manager;
    }

    template <typename T>
    static void shiftPointer(T *&pointer, int delta) {
        pointer = (T *)((byte *)pointer + delta);
    }

    ManagedObject *allocate(int size);
    void free(ManagedObject *object);

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);

    int getDelta();

private:
    MemoryManager();

    void shiftPointers();
};
