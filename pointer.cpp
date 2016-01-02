#include "pointer.h"

#include "memorymanager.h"
#include "hashtable.h"

template class Pointer<Object>;
template class Pointer<ManagedObject>;
template class Pointer<HashTable<std::string, Object *>>;
template class Pointer<HashTable<std::string, Object *>::HashNode>;

template <class T>
Pointer<T>::Pointer(T *p)
    : pointer(p), prev(0), next(0) {
    MemoryManager::instance()->registerPointer((Pointer<ManagedObject> *)this);
}

template <class T>
Pointer<T>::Pointer(const Pointer<T> &p)
    : pointer(p.pointer), prev(0), next(0) {
    MemoryManager::instance()->registerPointer((Pointer<ManagedObject> *)this);
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
