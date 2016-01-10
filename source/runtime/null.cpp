#include "null.h"

Null::Null() {
}

bool Null::isTrue() const {
    return false;
}

bool Null::isFalse() const {
    return false;
}

bool Null::isNull() const {
    return true;
}

std::string Null::toString() const {
    return "null";
}
