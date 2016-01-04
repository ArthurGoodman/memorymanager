#pragma once

#include <string>

#include "managedobject.h"

template <class, class>
class HashTable;

template <class>
class Pointer;

class Object : public ManagedObject {
    HashTable<uint, Object *> *attributes;

public:
    Object();

    bool hasAttribute(const std::string &name);
    Object *getAttribute(const std::string &name);
    void setAttribute(const std::string &name, const Pointer<Object> &value);
    void removeAttribute(const std::string &name);

    virtual bool equals(Object *object);
    virtual ulong hash();

    virtual std::string toString();

    virtual void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    virtual int getSize();
};
