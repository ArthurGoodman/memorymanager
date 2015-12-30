#pragma once

#include "object.h"

class String : public Object {
    std::string value;

public:
    String(std::string value);

    int getSize();

    std::string toString();
};
