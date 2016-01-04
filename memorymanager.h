#pragma once

#include "common.h"

class ManagedObject;

template <class>
class Pointer;

class MemoryManager {
    static MemoryManager *manager;

public:
    static MemoryManager *instance();

    static void initialize();
    static void finalize();

    template <class T>
    T *allocateArray(uint size);

    virtual ~MemoryManager();

    virtual ManagedObject *allocate(uint size, int count = 1) = 0;
    virtual void free(ManagedObject *p) = 0;

    virtual void collectGarbage() = 0;

    virtual void registerPointer(Pointer<ManagedObject> *pointer) = 0;
    virtual void removePointer(Pointer<ManagedObject> *pointer) = 0;
};

template <class T>
T *MemoryManager::allocateArray(uint size) {
    T *array = (T *)instance()->allocate(size * sizeof(T), size);
    return new (array) T[size];
}

inline MemoryManager *MemoryManager::instance() {
    return manager;
}
