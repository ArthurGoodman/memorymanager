#pragma once

#include "memorymanager.h"

template <class T>
class Pointer {
    T *pointer;
    Pointer<T> *prev, *next;

public:
    Pointer(T *p = 0);
    Pointer(const Pointer<T> &p);
    ~Pointer();

    Pointer<T> &operator=(T *p);
    Pointer<T> &operator=(const Pointer<T> &p);

    T *&operator*();
    T *operator->() const;
    operator T *() const;

    void link(Pointer<T> *&pointers);
    void unlink(Pointer<T> *&pointers);

    Pointer<T> *getNext();
};

template <class T>
Pointer<T>::Pointer(T *p)
    : pointer(p), prev(0), next(0) {
    MemoryManager::instance()->registerPointer((Pointer<ManagedObject> *)this);
}

template <class T>
Pointer<T>::Pointer(const Pointer<T> &p)
    : Pointer(p.pointer) {
}

template <class T>
Pointer<T>::~Pointer() {
    MemoryManager::instance()->removePointer((Pointer<ManagedObject> *)this);
}

template <class T>
Pointer<T> &Pointer<T>::operator=(const Pointer<T> &p) {
    this->pointer = p.pointer;
    return *this;
}

template <class T>
T *&Pointer<T>::operator*() {
    return pointer;
}

template <class T>
T *Pointer<T>::operator->() const {
    return pointer;
}

template <class T>
Pointer<T>::operator T *() const {
    return pointer;
}

template <class T>
Pointer<T> &Pointer<T>::operator=(T *p) {
    this->pointer = p;
    return *this;
}

template <class T>
void Pointer<T>::link(Pointer<T> *&pointers) {
    next = pointers;

    if (pointers)
        pointers->prev = this;

    pointers = this;
}

template <class T>
void Pointer<T>::unlink(Pointer<T> *&pointers) {
    if (next)
        next->prev = prev;

    if (prev)
        prev->next = next;

    if (pointers == this)
        pointers = pointers->next;
}

template <class T>
Pointer<T> *Pointer<T>::getNext() {
    return next;
}
