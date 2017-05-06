#pragma once

#include "object.h"

class False : public Object {
public:
    False();

    bool isTrue() const;
    bool isFalse() const;
    bool isNull() const;

    std::string toString() const;
};
