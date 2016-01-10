#include "true.h"

True::True() {
}

bool True::isTrue() const {
    return true;
}

bool True::isFalse() const {
    return false;
}

bool True::isNull() const {
    return false;
}

std::string True::toString() const {
    return "true";
}
