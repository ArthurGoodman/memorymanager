#pragma once

#include "memorymanager.h"

#include <iostream>

template <typename T>
class Pointer {
    friend class MemoryManager;

    const unsigned magic = 0xdeadbeef;

    T *pointer;
    Pointer<T> *prev, *next;

public:
    Pointer(T *p = 0)
        : pointer(p), prev(0), next(0) {
        MemoryManager::instance()->registerPointer((Pointer<ManagedObject> *)this);
    }

    Pointer(const Pointer<T> &p)
        : pointer(p.pointer), prev(0), next(0) {
        MemoryManager::instance()->registerPointer((Pointer<ManagedObject> *)this);
    }

    ~Pointer() {
        MemoryManager::instance()->removePointer((Pointer<ManagedObject> *)this);
    }

    Pointer<T> &operator=(T *p) {
        this->pointer = p;
        return *this;
    }

    Pointer<T> &operator=(const Pointer<T> &p) {
        this->pointer = p.pointer;
        return *this;
    }

    T *operator*() const {
        return pointer;
    }

    T *operator->() const {
        return pointer;
    }

    operator T *() const {
        return pointer;
    }
};
