#pragma once

#include "memorymanager.h"

class NoMemoryManager : public MemoryManager {
public:
    ManagedObject *allocate(uint size, int count);
    void free(ManagedObject *p);

    void collectGarbage();
};
