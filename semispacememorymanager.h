#pragma once

#include "memorymanager.h"
#include "bytearray.h"

class SemispaceMemoryManager : public MemoryManager {
    static const int InitialCapacity = 1 << 15;

    ByteArray memory;
    int objectCount, capacity;

    byte *toSpace, *fromSpace, *allocPtr;

public:
    SemispaceMemoryManager();

    ManagedObject *allocate(uint size, int count = 1);
    void free(ManagedObject *p);

    void collectGarbage();

private:
    bool enoughSpace(int size) const;
    ManagedObject *copy(ManagedObject *object);
    void expand();
};
