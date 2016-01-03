#pragma once

#include <functional>

#include "common.h"

class ManagedObject {
    friend class MemoryManager;

    static const int FlagMark = 1;

    uint flags;
    ManagedObject *forwardAddress;

public:
    static void *operator new(uint size);

    ManagedObject();
    virtual ~ManagedObject();

    void setFlag(int flag, bool value = true);

    virtual void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    virtual int getSize() = 0;

private:
    bool isMarked();
    void mark();
    void unmark();
};
