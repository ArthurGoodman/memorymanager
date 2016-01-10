#include "false.h"

False::False() {
}

bool False::isTrue() const {
    return false;
}

bool False::isFalse() const {
    return true;
}

bool False::isNull() const {
    return false;
}

std::string False::toString() const {
    return "false";
}
