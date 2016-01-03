#pragma once

#include "common.h"

class ManagedObject;

template <class>
class Pointer;

class IMemoryManager {
    static IMemoryManager *manager;

public:
    static IMemoryManager *instance();

    static void initialize();
    static void finalize();

    virtual ~IMemoryManager();

    virtual ManagedObject *allocate(uint size) = 0;
    virtual void free(ManagedObject *p) = 0;

    virtual void collectGarbage() = 0;

    virtual void registerPointer(Pointer<ManagedObject> *pointer) = 0;
    virtual void removePointer(Pointer<ManagedObject> *pointer) = 0;
};

inline IMemoryManager *IMemoryManager::instance() {
    return manager;
}
