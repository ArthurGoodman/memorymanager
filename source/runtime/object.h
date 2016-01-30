#pragma once

#include <list>

#include "managedobject.h"

template <class, class>
class Map;

template <class>
class Pointer;

class Class;

class Object : public ManagedObject {
    Class *objectClass;
    Map<uint, Object *> *attributes;

public:
    Object();
    Object(Class *objectClass);
    Object(std::string className);

    Class *getClass();
    void setClass(Class *objectClass);

    bool hasAttribute(const std::string &name) const;
    Object *getAttribute(const std::string &name) const;
    void setAttribute(const std::string &name, Object *value);
    void removeAttribute(const std::string &name);

    virtual bool hasAttribute(uint id) const;
    virtual Object *getAttribute(uint id) const;
    virtual void setAttribute(uint id, Object *value);
    virtual void removeAttribute(uint id);

    int attributesCount() const;

    bool instanceOf(Class *_class);

    virtual Object *call(const std::string &name, const std::list<Object *> &args);
    virtual Object *call(const std::string &name, Object *arg);
    virtual Object *call(const std::string &name);

    virtual bool isTrue() const;
    virtual bool isFalse() const;
    virtual bool isNull() const;

    virtual bool equals(Object *object) const;
    virtual std::string toString() const;
    virtual ulong hash() const;

    virtual void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    virtual int getSize() const;
};
