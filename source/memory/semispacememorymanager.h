#pragma once

#include "memorymanager.h"
#include "bytearray.h"

class SemispaceMemoryManager : public MemoryManager {
    static const int InitialCapacity = 1 << 10;

    ByteArray memory;
    int objectCount, memoryUsed, capacity, delta;

    byte *toSpace, *fromSpace, *allocPtr;

public:
    SemispaceMemoryManager();
    ~SemispaceMemoryManager();

    ManagedObject *allocate(uint size, int count = 1);
    void free(ManagedObject *p);

    void collectGarbage();

private:
    void initialize();
    void finalize();

    bool enoughSpace(int size) const;

    ManagedObject *copy(ManagedObject *object);
    void expand();
};
