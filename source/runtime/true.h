#pragma once

#include "object.h"

class True : public Object {
public:
    True();

    bool isTrue() const;
    bool isFalse() const;
    bool isNull() const;

    std::string toString() const;
};
