#pragma once

#include "memorymanager.h"

class NoMemoryManager : public MemoryManager {
public:
    ManagedObject *allocate(uint size, int count);
    void free(ManagedObject *p);

    void collectGarbage();

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);
};
