#pragma once

class ManagedObject;

template <class>
class Pointer;

class LocalMemoryManager;
#define MemoryManager LocalMemoryManager

class IMemoryManager {
    static MemoryManager manager;

public:
    static IMemoryManager *instance();

    virtual ManagedObject *allocate(int size) = 0;

    virtual void collectGarbage() = 0;

    virtual void registerPointer(Pointer<ManagedObject> *pointer) = 0;
    virtual void removePointer(Pointer<ManagedObject> *pointer) = 0;
};

inline IMemoryManager *IMemoryManager::instance() {
    return (IMemoryManager *)&manager;
}
