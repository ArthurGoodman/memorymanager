#include "pointer.h"

#include "imemorymanager.h"
#include "hashtable.h"

template class Pointer<Object>;
template class Pointer<ManagedObject>;
template class Pointer<HashTable<uint, Object *>>;
template class Pointer<HashTable<uint, Object *>::Entry>;
template class Pointer<HashTable<Object *, uint>>;
template class Pointer<HashTable<Object *, uint>::Entry>;

template <class T>
Pointer<T>::Pointer(T *p)
    : pointer(p), prev(0), next(0) {
    IMemoryManager::instance()->registerPointer((Pointer<ManagedObject> *)this);
}

template <class T>
Pointer<T>::Pointer(const Pointer<T> &p)
    : Pointer(p.pointer) {
}

template <class T>
Pointer<T>::~Pointer() {
    IMemoryManager::instance()->removePointer((Pointer<ManagedObject> *)this);
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
