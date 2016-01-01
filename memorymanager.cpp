#include "memorymanager.h"

#include "managedobject.h"
#include "pointer.h"

MemoryManager MemoryManager::manager;

MemoryManager *MemoryManager::instance() {
    return &manager;
}

ManagedObject *MemoryManager::allocate(int size) {
    if (!memory.enoughSpace(size))
        collectGarbage();

    byte *oldAddress = memory.getData();
    ManagedObject *object = (ManagedObject *)memory.allocate(size);
    delta = memory.getData() - oldAddress;

    if (delta != 0)
        shiftPointers();

    objectCount++;

    return object;
}

void MemoryManager::registerPointer(Pointer<ManagedObject> *p) {
    p->next = firstPointer;

    if (firstPointer)
        firstPointer->prev = p;

    firstPointer = p;
}

void MemoryManager::removePointer(Pointer<ManagedObject> *p) {
    if (p->next)
        p->next->prev = p->prev;

    if (p->prev)
        p->prev->next = p->next;

    if (firstPointer == p)
        firstPointer = firstPointer->next;
}

int MemoryManager::getDelta() {
    return delta;
}

MemoryManager::MemoryManager()
    : delta(0), objectCount(0), firstPointer(0) {
    Vector::setInitialCapacity(128);
}

void MemoryManager::shiftPointers() {
    byte *objects = memory.getData();

    for (int i = 0; i < objectCount; i++) {
        ((ManagedObject *)objects)->shiftPointers(delta);
        objects += ((ManagedObject *)objects)->getSize();
    }

    Pointer<ManagedObject> *p = firstPointer;

    while (p) {
        if (p->pointer)
            shiftPointer(p->pointer, delta);

        p = p->next;
    }
}

void MemoryManager::collectGarbage() {
    mark();
    sweep();
    defragment();
}

void MemoryManager::mark() {
    Pointer<ManagedObject> *p = firstPointer;

    while (p) {
        if (p->pointer)
            (*p)->mark();

        p = p->next;
    }
}

void MemoryManager::sweep() {
}

void MemoryManager::defragment() {
}
