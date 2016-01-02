#pragma once

#include "common.h"

class ManagedObject {
    friend class MemoryManager;

    uint flags;
    byte *forwardAddress;

public:
    enum Flag {
        FlagMark = 1 << 0,
    };

    static void *operator new(uint size);

    ManagedObject();
    virtual ~ManagedObject();

    void setFlag(Flag flag, bool value = true);

    bool isMarked();
    void unmark();

    byte *getForwardAddress();

    virtual void shiftPointers();
    virtual void forwardPointers();
    virtual void mark();

    virtual int getSize() = 0;
};
