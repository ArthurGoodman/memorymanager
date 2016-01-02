#include "vector.h"

#include <memory>
#include <cmath>

#include <iostream>

int Vector::initialCapacity = 2;

void Vector::setInitialCapacity(int initialCapacity) {
    Vector::initialCapacity = initialCapacity;
}

Vector::Vector()
    : size(0), capacity(0), data(0) {
}

Vector::~Vector() {
    release();
}

byte *Vector::allocate(int count) {
    if (count < 0)
        return 0;

    if (!enoughSpace(count)) {
        std::cout << "//reallocating\n";

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

bool Vector::free(int count) {
    if (count < 0 || size < count)
        return false;

    size -= count;
    return true;
}

void Vector::release() {
    if (data)
        ::free(data);

    size = 0;
    capacity = 0;
    data = 0;
}

bool Vector::enoughSpace(int count) const {
    return size + count <= capacity;
}

byte *Vector::getData() const {
    return data;
}

int Vector::getSize() const {
    return size;
}

int Vector::getCapacity() const {
    return capacity;
}
