#pragma once

#include "object.h"

class String : public Object {
    std::string value;

public:
    String(std::string value);

    bool equals(Object *object);
    ulong hash();

    std::string toString();

protected:
    int getSize();
};
