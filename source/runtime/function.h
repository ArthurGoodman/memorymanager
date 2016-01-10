#pragma once

#include "object.h"

class Function : public Object {
    std::string name;

public:
    Function(const std::string &name);

    virtual Object *invoke(Object *self, const std::list<Object *> &args) = 0;

    virtual std::string toString() const;
};
