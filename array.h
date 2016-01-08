#pragma once

#include "object.h"
#include "memorymanager.h"

template <class T>
class Array : public Object {
    int size;

public:
    class iterator {
        friend class Array<T>;

        T *data;
        int size, i;

    public:
        iterator &operator++();
        T &operator*();

        bool operator!=(const iterator &other) const;

    private:
        iterator();
        iterator(T *data, int size, int i);
    };

    static Array *create(int size);

    iterator begin();
    iterator end();

    T *data() const;

    T *operator*() const;
    T &operator[](int index) const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;

private:
    Array(int size);
};

template <class T>
typename Array<T>::iterator &Array<T>::iterator::operator++() {
    i++;
    return *this;
}

template <class T>
T &Array<T>::iterator::operator*() {
    return data[i];
}

template <class T>
bool Array<T>::iterator::operator!=(const iterator &other) const {
    return i != other.i;
}

template <class T>
Array<T>::iterator::iterator(T *data, int size, int i)
    : data(data), size(size), i(i) {
}

template <class T>
Array<T> *Array<T>::create(int size) {
    Array<T> *array = (Array<T> *)MemoryManager::instance()->allocate(sizeof(Array) + size * sizeof(T));
    new (array) Array<T>(size);
    new (**array) T[size];
    return array;
}

template <class T>
typename Array<T>::iterator Array<T>::begin() {
    return iterator(data(), size, 0);
}

template <class T>
typename Array<T>::iterator Array<T>::end() {
    return iterator(data(), size, size);
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
void Array<T>::mapOnReferences(const std::function<void(ManagedObject *&)> &) {
}

template <class T>
int Array<T>::getSize() const {
    return sizeof(*this) + size * sizeof(T);
}

template <class T>
Array<T>::Array(int size)
    : size(size) {
}
