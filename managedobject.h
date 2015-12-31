#pragma once

#include "common.h"

class ManagedObject {
    int flags;

public:
    enum Flag {
        FlagFree = 1 << 0,
        FlagMark = 1 << 1,
    };

    static void *operator new(uint size);
    static void operator delete(void *p);

    ManagedObject();
    virtual ~ManagedObject();

    void setFlag(Flag flag, bool value = true);

    bool isFree();

    virtual void shiftPointers(int delta);
    virtual void mark();

    virtual int getSize() = 0;
};
