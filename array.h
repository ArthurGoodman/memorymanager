#pragma once

#include "managedobject.h"
#include "memorymanager.h"

class Array {
    int size;

public:
    template <class T>
    static Array *create(int size);

    void *data() const;

    int getSize() const;

private:
    Array(int size);
};

template <class T>
Array *Array::create(int size) {
    ManagedObject *data = MemoryManager::instance()->allocate(sizeof(Array) + size * sizeof(T));
    new (data) Array(size * sizeof(T));
    return (Array *)data;
}
