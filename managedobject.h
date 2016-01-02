#pragma once

#include <list>
#include <functional>

#include "common.h"

class ManagedObject;

typedef std::list<std::reference_wrapper<ManagedObject *>> References;

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

    virtual void getReferences(References &references);

    virtual int getSize() = 0;
};
