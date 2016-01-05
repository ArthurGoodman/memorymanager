#pragma once

#include "memorymanager.h"
#include "bytearray.h"

class MarkCompactMemoryManager : public MemoryManager {
    ByteArray memory;
    Pointer<ManagedObject> *pointers;
    int delta, objectCount;

public:
    MarkCompactMemoryManager();
    ~MarkCompactMemoryManager();

    ManagedObject *allocate(uint size, int count);
    void free(ManagedObject *p);

    void collectGarbage();

    void registerPointer(Pointer<ManagedObject> *pointer);
    void removePointer(Pointer<ManagedObject> *pointer);

private:
    void shiftPointers();
    void shiftPointer(ManagedObject *&pointer);

    void mark();
    void compact();

    void finalize();

    void shiftPointers(ManagedObject *object);
    void forwardPointers(ManagedObject *object);
    void mark(ManagedObject *object);
};
