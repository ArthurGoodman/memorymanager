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

    virtual void mapOnReferences(void (*f)(ManagedObject *&));
    virtual int getSize();

    bool hasAttribute(const std::string &name);
    Object *getAttribute(const std::string &name);
    void setAttribute(const std::string &name, const Pointer<Object> &value);
    void removeAttribute(const std::string &name);

    virtual std::string toString();
};
