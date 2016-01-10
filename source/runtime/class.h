#pragma once

#include "object.h"

class Class : public Object {
    std::string name;
    Class *superclass;

public:
    Class(const std::string &name, Class *superclass);
    Class(const std::string &name, const std::string &superclassName);
    Class(const std::string &name);

    std::string getName();

    Class *getSuperclass();
    void setSuperclass(Class *superclass);

    bool isChild(Class *_class);

    Object *newInstance();

    Object *lookup(std::string name);

    virtual std::string toString() const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;

protected:
    virtual Object *createNewInstance();
};
