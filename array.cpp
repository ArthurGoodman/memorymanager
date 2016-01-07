#include "array.h"

void *Array::data() const {
    return (void *)(this + size);
}

int Array::getSize() const {
    return sizeof(*this) + size;
}

Array::Array(int size)
    : size(size) {
}
