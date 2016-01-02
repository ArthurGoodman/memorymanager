#pragma once

#include "common.h"

class ManagedObject {
    friend class MemoryManager;

    static const int FlagMark = 1;

    uint flags;
    byte *forwardAddress;

public:
    static void *operator new(uint size);

    ManagedObject();
    virtual ~ManagedObject();

    void setFlag(int flag, bool value = true);

    bool isMarked();
    void mark();
    void unmark();

    byte *getForwardAddress();

    virtual void mapOnReferences(void (*f)(ManagedObject *&));
    virtual int getSize() = 0;
};
