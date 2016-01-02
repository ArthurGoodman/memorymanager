#include "memorymanager.h"

#include <memory>

#include "managedobject.h"
#include "pointer.h"

#include <iostream>

MemoryManager MemoryManager::manager;

MemoryManager *MemoryManager::instance() {
    return &manager;
}

ManagedObject *MemoryManager::allocate(int size) {
    if (!memory.enoughSpace(size))
        collectGarbage();

    byte *oldAddress = memory.getData();

    ManagedObject *object = (ManagedObject *)memory.allocate(size);

    if ((delta = memory.getData() - oldAddress) != 0)
        shiftPointers();

    objectCount++;

    return object;
}

void MemoryManager::registerPointer(Pointer<ManagedObject> *p) {
    p->next = pointers;

    if (pointers)
        pointers->prev = p;

    pointers = p;
}

void MemoryManager::removePointer(Pointer<ManagedObject> *p) {
    if (p->next)
        p->next->prev = p->prev;

    if (p->prev)
        p->prev->next = p->next;

    if (pointers == p)
        pointers = pointers->next;
}

MemoryManager::MemoryManager()
    : delta(0), objectCount(0), pointers(0) {
    Vector::setInitialCapacity(128);
}

void MemoryManager::shiftPointers() {
    std::cout << "MemoryManager::shiftPointers() //delta=" << delta << "\n";

    byte *p = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        ((ManagedObject *)p)->shiftPointers();

    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p)
            shiftPointer(**p);
}

void MemoryManager::collectGarbage() {
    std::cout << "MemoryManager::collectGarbage()\n";

    mark();
    compact();
}

void MemoryManager::mark() {
    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p && !(*p)->isMarked())
            (*p)->mark();
}

void MemoryManager::compact() {
    int freeCount = 0, freeSize = 0;

    byte *p, *free;
    p = free = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        if (((ManagedObject *)p)->isMarked()) {
            ((ManagedObject *)p)->forwardAddress = free;
            free += ((ManagedObject *)p)->getSize();
        } else {
            freeSize += ((ManagedObject *)p)->getSize();
            freeCount++;
        }

    p = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        if (((ManagedObject *)p)->isMarked())
            ((ManagedObject *)p)->forwardPointers();

    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p && (*p)->isMarked())
            **p = (ManagedObject *)(*p)->forwardAddress;

    p = memory.getData();

    for (int i = 0, size = 0; i < objectCount; i++, p += size) {
        size = ((ManagedObject *)p)->getSize();

        if (((ManagedObject *)p)->isMarked()) {
            byte *destination = ((ManagedObject *)p)->forwardAddress;

            memmove(destination, p, ((ManagedObject *)p)->getSize());

            ((ManagedObject *)destination)->unmark();
            ((ManagedObject *)destination)->forwardAddress = 0;
        }
    }

    memory.free(freeSize);
    objectCount -= freeCount;
}
