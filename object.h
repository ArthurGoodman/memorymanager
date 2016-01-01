#pragma once

#include <string>

#include "managedobject.h"

template <class, class>
class HashTable;

template <class>
class Pointer;

class Object : public ManagedObject {
    HashTable<std::string, Object *> *attributes;

public:
    Object();

    virtual void shiftPointers(int delta);
    virtual void mark();

    virtual int getSize();

    bool hasAttribute(std::string name);
    Object *getAttribute(std::string name);
    void setAttribute(std::string name, const Pointer<Object> &value);

    virtual std::string toString();
};
