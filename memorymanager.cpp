#include "memorymanager.h"

#include <memory>

#include "managedobject.h"
#include "pointer.h"

#include <iostream>

MemoryManager MemoryManager::manager;

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

void MemoryManager::collectGarbage() {
    std::cout << "\nMemoryManager::collectGarbage()\n";

    int oldSize = memory.getSize(), oldObjectCount = objectCount;

    mark();
    compact();

    std::cout << "//freed=" << oldSize - memory.getSize() << ", freedObjects=" << oldObjectCount - objectCount << ", objectCount=" << objectCount << "\n\n";
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
    Vector::setInitialCapacity(1024);
}

MemoryManager::~MemoryManager() {
    release();
}

void MemoryManager::shiftPointers() {
    std::cout << "MemoryManager::shiftPointers() //delta=" << delta << "\n\n";

    byte *p = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        shiftPointers((ManagedObject *)p);

    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p)
            shiftPointer(**p);
}

void MemoryManager::mark() {
    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p && !(*p)->isMarked())
            mark(*p);
}

void MemoryManager::compact() {
    byte *p, *free;
    p = free = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        if (((ManagedObject *)p)->isMarked()) {
            ((ManagedObject *)p)->forwardAddress = (ManagedObject *)free;
            free += ((ManagedObject *)p)->getSize();
        }

    p = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        if (((ManagedObject *)p)->isMarked())
            forwardPointers((ManagedObject *)p);

    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p && (*p)->isMarked())
            **p = (*p)->forwardAddress;

    p = memory.getData();

    int freeCount = 0, freeSize = 0;

    for (int i = 0, size = 0; i < objectCount; i++, p += size) {
        size = ((ManagedObject *)p)->getSize();

        if (((ManagedObject *)p)->isMarked()) {
            byte *dst = (byte *)((ManagedObject *)p)->forwardAddress;

            memmove(dst, p, size);

            ((ManagedObject *)dst)->unmark();
            ((ManagedObject *)dst)->forwardAddress = 0;
        } else {
            freeSize += size;
            freeCount++;

            ((ManagedObject *)p)->~ManagedObject();
        }
    }

    memory.free(freeSize);
    objectCount -= freeCount;
}

void MemoryManager::release() {
    std::cout << "\nMemoryManager::release()\n";

    byte *p = memory.getData();

    for (int i = 0, size = 0; i < objectCount; i++, p += size) {
        size = ((ManagedObject *)p)->getSize();
        ((ManagedObject *)p)->~ManagedObject();
    }
}

void MemoryManager::shiftPointers(ManagedObject *object) {
    object->mapOnReferences([](ManagedObject *&ref) {
        shiftPointer(ref);
    });
}

void MemoryManager::forwardPointers(ManagedObject *object) {
    object->mapOnReferences([](ManagedObject *&ref) {
        ref = ref->forwardAddress;
    });
}

void MemoryManager::mark(ManagedObject *object) {
    object->mark();

    object->mapOnReferences([](ManagedObject *&ref) {
        if (!ref->isMarked())
            MemoryManager::instance()->mark(ref);
    });
}
