#pragma once

#include "managedobject.h"

class StaticObject : public ManagedObject {
    int refCount;

public:
    static StaticObject *create(int refCount);

    StaticObject(int refCount);

    ManagedObject *&field(int index);

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;
};
