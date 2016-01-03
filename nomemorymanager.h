#pragma once

#include "imemorymanager.h"

class NoMemoryManager : public IMemoryManager {
public:
    ManagedObject *allocate(uint size);
    void free(ManagedObject *p);

    void collectGarbage();

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);
};
