#pragma once

#include "managedobject.h"

class EmptyObject : public ManagedObject {
    int size;

public:
    EmptyObject(int size);

    int getSize();
};
