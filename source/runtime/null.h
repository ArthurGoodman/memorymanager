#pragma once

#include "object.h"

class Null : public Object {
public:
    Null();

    bool isTrue() const;
    bool isFalse() const;
    bool isNull() const;

    std::string toString() const;
};
