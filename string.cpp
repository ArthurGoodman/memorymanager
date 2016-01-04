#include "string.h"

#include <iostream>

String::String(std::string value)
    : value(value) {
    std::cout << "String::String(value=" << value << ")\n";
}

bool String::equals(Object *object) {
    return dynamic_cast<String *>(object) && ((String *)object)->value == value;
}

ulong String::hash() {
    return std::hash<std::string>()(value);
}

std::string String::toString() {
    return value;
}

int String::getSize() {
    return sizeof *this;
}
