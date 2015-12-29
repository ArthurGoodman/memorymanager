#include "pointer.h"

#include "object.h"
#include "memorymanager.h"

#include <iostream>

Pointer::Pointer(Object *object)
    : object(object), prev(0), next(0) {
    MemoryManager::instance()->registerPointer(this);
}

Pointer::~Pointer() {
    MemoryManager::instance()->removePointer(this);
}

void Pointer::shift(int delta) {
    //    std::cout << "Pointer::shift{this=" << this << ", delta=" << delta << "}\n";

    if (object)
        object = (Object *)((byte *)object + delta);
}

Object *Pointer::operator*() {
    return object;
}

Object *Pointer::operator->() {
    return object;
}

Pointer::operator Object *() {
    return object;
}
