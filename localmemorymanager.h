#pragma once

#include "imemorymanager.h"
#include "bytearray.h"

class LocalMemoryManager : public IMemoryManager {
    ByteArray memory;
    int delta;

    int objectCount;

    Pointer<ManagedObject> *pointers;

public:
    LocalMemoryManager();
    ~LocalMemoryManager();

    ManagedObject *allocate(uint size);
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
