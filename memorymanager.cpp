#include "memorymanager.h"

#include <memory>

#include "managedobject.h"
#include "pointer.h"

#include <iostream>

MemoryManager MemoryManager::manager;

MemoryManager *MemoryManager::instance() {
    return &manager;
}

int MemoryManager::getDelta() {
    return manager.delta;
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

    byte *objects = memory.getData();

    for (int i = 0; i < objectCount; i++, objects += ((ManagedObject *)objects)->getSize())
        ((ManagedObject *)objects)->shiftPointers(delta);

    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p)
            shiftPointer(**p, delta);
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
    int liveCount = 0, freeSize = 0;

    byte *p, *free;
    p = free = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        if (((ManagedObject *)p)->isMarked()) {
            ((ManagedObject *)p)->forwardAddress = free;
            free += ((ManagedObject *)p)->getSize();

            liveCount++;
        } else
            freeSize += ((ManagedObject *)p)->getSize();

    p = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        if (((ManagedObject *)p)->isMarked() && ((ManagedObject *)p)->forwardAddress != p)
            ((ManagedObject *)p)->shiftPointers(((ManagedObject *)p)->forwardAddress - p);

    p = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        if (((ManagedObject *)p)->isMarked() && ((ManagedObject *)p)->forwardAddress != p) {
            memcpy(((ManagedObject *)p)->forwardAddress, p, ((ManagedObject *)p)->getSize());

            ((ManagedObject *)((ManagedObject *)p)->forwardAddress)->unmark();
            ((ManagedObject *)((ManagedObject *)p)->forwardAddress)->forwardAddress = 0;
        }

    memory.free(freeSize);
    objectCount = liveCount;
}
