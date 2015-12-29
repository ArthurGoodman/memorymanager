#include "memorymanager.h"

#include "managedobject.h"
#include "pointer.h"

#include <iostream>

MemoryManager *MemoryManager::manager = new MemoryManager;

ManagedObject *MemoryManager::allocate(int size) {
    byte *oldAddress = memory.getData();
    ManagedObject *object = (ManagedObject *)memory.allocate(size);
    delta = memory.getData() - oldAddress;

    if (delta != 0)
        shiftPointers();

    objectCount++;

    return object;
}

void MemoryManager::free(ManagedObject *object) {
    object->setFlag(ManagedObject::FlagFree);
}

void MemoryManager::registerPointer(Pointer *p) {
    std::cout << "MemoryManager::registerPointer{this=" << this << ", p=" << p << "}\n";

    p->next = firstPointer;

    if (firstPointer)
        firstPointer->prev = p;

    firstPointer = p;
}

void MemoryManager::removePointer(Pointer *p) {
    std::cout << "MemoryManager::removePointer{this=" << this << ", p=" << p << "}\n";

    if (p->next)
        p->next->prev = p->prev;

    if (p->prev)
        p->prev->next = p->next;

    if (firstPointer == p)
        firstPointer = firstPointer->next;
}

MemoryManager::MemoryManager()
    : delta(0), objectCount(0), firstPointer(0) {
}

void MemoryManager::shiftPointers() {
    std::cout << "MemoryManager::shiftPointers{this=" << this << "}\n";

    //    byte *objects = memory.getData();

    //    for (int i = 0; i < objectCount - 1; i++) {
    //        ((Object *)objects)->shiftPointers();
    //        objects += ((Object *)objects)->getSize();
    //    }

    Pointer *p = firstPointer;

    while (p) {
        if (p->magic != 0xdeadbeef) {
            p = shiftPointer(p);

            if (p->prev)
                p->prev->next = p;

            if (p->next)
                p->next->prev = p;
        }

        p->shift(delta);
        p = p->next;
    }
}
