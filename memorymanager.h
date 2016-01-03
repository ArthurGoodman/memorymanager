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

    template <class T>
    static void shiftPointer(T *&pointer);

    ManagedObject *allocate(int size);

    void collectGarbage();

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);

private:
    MemoryManager();
    ~MemoryManager();

    void shiftPointers();

    void mark();
    void compact();

    void release();

    void shiftPointers(ManagedObject *object);
    void forwardPointers(ManagedObject *object);
    void mark(ManagedObject *object);
};

inline MemoryManager *MemoryManager::instance() {
    return &manager;
}

template <class T>
inline void MemoryManager::shiftPointer(T *&pointer) {
    pointer = (T *)((byte *)pointer + manager.delta);
}
