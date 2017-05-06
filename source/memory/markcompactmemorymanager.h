#pragma once

#include "memorymanager.h"
#include "bytearray.h"

class MarkCompactMemoryManager : public MemoryManager {
    ByteArray memory;
    int objectCount, delta;

public:
    MarkCompactMemoryManager();
    ~MarkCompactMemoryManager();

    ManagedObject *allocate(uint size, int count);
    void free(ManagedObject *p);

    void collectGarbage();

private:
    void initialize();
    void finalize();

    void updatePointers();
    void updatePointer(ManagedObject *&pointer);

    void mark();
    void compact();

    void updatePointers(ManagedObject *object);
    void forwardPointers(ManagedObject *object);
    void mark(ManagedObject *object);
};

inline void MarkCompactMemoryManager::updatePointer(ManagedObject *&pointer) {
    pointer = (ManagedObject *)((byte *)pointer + delta);
}
