#pragma once

#include "common.h"

class ManagedObject;

template <class>
class Pointer;

class MemoryManager {
    static MemoryManager *manager;

protected:
    static Pointer<ManagedObject> *pointers;

public:
    static MemoryManager *instance();

    static void initialize();
    static void finalize();

    static void registerPointer(Pointer<ManagedObject> *pointer);
    static void removePointer(Pointer<ManagedObject> *pointer);

    template <class T>
    T *allocateArray(uint size);

    virtual ~MemoryManager();

    virtual ManagedObject *allocate(uint size, int count = 1) = 0;
    virtual void free(ManagedObject *p) = 0;

    virtual void collectGarbage() = 0;
};

template <class T>
T *MemoryManager::allocateArray(uint size) {
    return new (instance()->allocate(size * sizeof(T), size)) T[size];
}

inline MemoryManager *MemoryManager::instance() {
    return manager;
}
