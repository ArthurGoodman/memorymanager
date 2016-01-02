#pragma once

#include "object.h"

class String : public Object {
    std::string value;

public:
    String(std::string value);

    std::string toString();

protected:
    int getSize();
};
