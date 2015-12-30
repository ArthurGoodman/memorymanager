#include "string.h"

String::String(std::string value)
    : value(value) {
}

int String::getSize() {
    return sizeof *this;
}

std::string String::toString() {
    return value;
}
