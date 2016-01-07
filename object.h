#pragma once

#include "managedobject.h"

template <class, class>
class Map;

template <class>
class Pointer;

class Object : public ManagedObject {
    Map<uint, Object *> *attributes;

public:
    Object();

    bool hasAttribute(const std::string &name);
    Object *getAttribute(const std::string &name);
    void setAttribute(const std::string &name, Object *value);
    void removeAttribute(const std::string &name);

    bool hasAttribute(uint id);
    Object *getAttribute(uint id);
    void setAttribute(uint id, const Pointer<Object> &value);
    void removeAttribute(uint id);

    virtual bool equals(Object *object) const;
    virtual ulong hash() const;

    virtual std::string toString() const;

    virtual void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    virtual int getSize() const;

    int attributesCount() const;
};
