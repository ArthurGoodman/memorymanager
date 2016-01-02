#include "string.h"

#include <iostream>

String::String(std::string value)
    : value(value) {
    std::cout << "String::String(value=" << value << ")\n";
}

String::~String() {
    std::cout << "String::~String() //value=" << value << "\n";
}

std::string String::toString() {
    return value;
}

int String::getSize() {
    return sizeof *this;
}
