#include "vector.h"

#include <memory>
#include <cmath>

Vector::Vector()
    : size(0), capacity(0), data(0) {
}

Vector::Vector(const Vector &v)
    : size(0), capacity(0), data(0) {
    allocate(v.getSize());
    memcpy(data, v.getData(), size);
}

Vector::~Vector() {
    release();
}

byte *Vector::allocate(int count) {
    if (count <= 0)
        return 0;

    if (!enoughSpace(count)) {
        int newCapacity = capacity;

        if (!newCapacity)
            newCapacity = InitialCapacity;

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
    if (count <= 0 || size < count)
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
