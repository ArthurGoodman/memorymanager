#pragma once

#include "object.h"

class Function : public Object {
    std::string name;

public:
    Function(const std::string &name);

    Object *invoke(Object *self, const std::list<Object *> &args);

    virtual std::string toString();
};
