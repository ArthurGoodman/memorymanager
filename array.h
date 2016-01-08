#pragma once

#include "object.h"
#include "memorymanager.h"

template <class T>
class Array : public Object {
    int size;

public:
    static Array *create(int size);

    T *data() const;

    T *operator*() const;
    T &operator[](int index) const;

    int getSize() const;

private:
    Array(int size);
};

template <class T>
Array<T> *Array<T>::create(int size) {
    Array<T> *array = (Array<T> *)MemoryManager::instance()->allocate(sizeof(Array) + size * sizeof(T));
    new (array) Array<T>(size * sizeof(T));
    new (**array) T[size];
    return array;
}

template <class T>
T *Array<T>::data() const {
    return (T *)(this + 1);
}

template <class T>
T *Array<T>::operator*() const {
    return data();
}

template <class T>
T &Array<T>::operator[](int index) const {
    return data()[index];
}

template <class T>
int Array<T>::getSize() const {
    return sizeof(*this) + size;
}

template <class T>
Array<T>::Array(int size)
    : size(size) {
}
