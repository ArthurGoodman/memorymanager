#include "string.h"

String::String(std::string value)
    : value(value) {
}

std::string String::toString() {
    return value;
}

int String::getSize() {
    return sizeof *this;
}
