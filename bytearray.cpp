#include "bytearray.h"

#include <memory>
#include <cmath>

#include <iostream>

uint ByteArray::initialCapacity = 1;

void ByteArray::setInitialCapacity(uint initialCapacity) {
    ByteArray::initialCapacity = initialCapacity;
}

ByteArray::ByteArray()
    : size(0), capacity(0), data(0) {
}

ByteArray::~ByteArray() {
    release();
}

byte *ByteArray::allocate(uint count) {
    if (!enoughSpace(count)) {
        std::cout << "//reallocating\n\n";

        int newCapacity = capacity;

        if (!newCapacity)
            newCapacity = initialCapacity;

        newCapacity *= pow(2, std::max(0.0, ceil(log2(double(size + count) / newCapacity))));

        byte *newData = (byte *)realloc(data, newCapacity);

        if (!newData)
            return 0;

        capacity = newCapacity;
        data = newData;
    }

    size += count;

    return data + size - count;
}

bool ByteArray::free(uint count) {
    if (size < count)
        return false;

    size -= count;
    return true;
}

void ByteArray::release() {
    if (data)
        ::free(data);

    size = 0;
    capacity = 0;
    data = 0;
}

bool ByteArray::enoughSpace(uint count) const {
    return size + count <= capacity;
}

byte *ByteArray::getData() const {
    return data;
}

int ByteArray::getSize() const {
    return size;
}

int ByteArray::getCapacity() const {
    return capacity;
}
