#pragma once

#include <string>

#include "common.h"
#include "managedobject.h"
#include "pointer.h"

template <typename K, typename V>
class HashTable;

class Object : public ManagedObject {
    HashTable<std::string, Object *> *attributes;

public:
    Object();

    virtual void shiftPointers(int delta);
    virtual void shiftPointersAgain(int delta);

    virtual void mark();
    virtual int getSize();

    bool hasAttribute(std::string name);
    Object *getAttribute(std::string name);
    void setAttribute(std::string name, Object *value);

    virtual std::string toString();
};
