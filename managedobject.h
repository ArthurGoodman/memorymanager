#pragma once

#include "common.h"

class ManagedObject {
    friend class MemoryManager;

    uint flags;
    byte *forwardAddress;

public:
    static void *operator new(uint size);

    ManagedObject();
    virtual ~ManagedObject();

    bool isMarked();
    void unmark();

    virtual void shiftPointers(int delta);
    virtual void mark();

    virtual int getSize() = 0;

private:
    enum Flag {
        FlagMark = 1 << 0,
    };

    void setFlag(Flag flag, bool value = true);
};
