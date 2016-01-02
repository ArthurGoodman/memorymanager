#pragma once

#include <string>

#include "managedobject.h"

class HashTable;

template <class>
class Pointer;

class Object : public ManagedObject {
    HashTable *attributes;

public:
    Object();

    virtual void shiftPointers();
    virtual void forwardPointers();
    virtual void mark();

    virtual int getSize();

    bool hasAttribute(std::string name);
    Object *getAttribute(std::string name);
    void setAttribute(std::string name, const Pointer<Object> &value);

    virtual std::string toString();
};
