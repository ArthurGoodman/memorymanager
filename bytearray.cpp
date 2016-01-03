#include "bytearray.h"

#include <memory>
#include <cmath>

#include <iostream>

int ByteArray::initialCapacity = 1;

void ByteArray::setInitialCapacity(int initialCapacity) {
    ByteArray::initialCapacity = initialCapacity;
}

ByteArray::ByteArray()
    : size(0), capacity(0), data(0) {
}

ByteArray::~ByteArray() {
    release();
}

byte *ByteArray::allocate(int count) {
    if (count < 0)
        return 0;

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

bool ByteArray::free(int count) {
    if (count < 0 || size < count)
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

bool ByteArray::enoughSpace(int count) const {
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
