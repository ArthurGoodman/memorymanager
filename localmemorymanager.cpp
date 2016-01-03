#include "localmemorymanager.h"

#include <memory>

#include "managedobject.h"
#include "pointer.h"

#include <iostream>

LocalMemoryManager::LocalMemoryManager()
    : delta(0), objectCount(0), pointers(0) {
    ByteArray::setInitialCapacity(1024);
}

LocalMemoryManager::~LocalMemoryManager() {
    release();
}

ManagedObject *LocalMemoryManager::allocate(int size) {
    if (!memory.enoughSpace(size))
        collectGarbage();

    byte *oldAddress = memory.getData();

    ManagedObject *object = (ManagedObject *)memory.allocate(size);

    if ((delta = memory.getData() - oldAddress) != 0)
        shiftPointers();

    objectCount++;

    return object;
}

void LocalMemoryManager::collectGarbage() {
    std::cout << "\nMemoryManager::collectGarbage()\n";

    int oldSize = memory.getSize(), oldObjectCount = objectCount;

    mark();
    compact();

    std::cout << "//freed=" << oldSize - memory.getSize() << ", freedObjects=" << oldObjectCount - objectCount << ", objectCount=" << objectCount << "\n\n";
}

void LocalMemoryManager::registerPointer(Pointer<ManagedObject> *p) {
    p->next = pointers;

    if (pointers)
        pointers->prev = p;

    pointers = p;
}

void LocalMemoryManager::removePointer(Pointer<ManagedObject> *p) {
    if (p->next)
        p->next->prev = p->prev;

    if (p->prev)
        p->prev->next = p->next;

    if (pointers == p)
        pointers = pointers->next;
}

void LocalMemoryManager::shiftPointers() {
    std::cout << "MemoryManager::shiftPointers() //delta=" << delta << "\n\n";

    byte *p = memory.getData();

    for (int i = 0; i < objectCount; i++, p += ((ManagedObject *)p)->getSize())
        shiftPointers((ManagedObject *)p);

    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p)
            shiftPointer(**p);
}

void LocalMemoryManager::shiftPointer(ManagedObject *&pointer) {
    pointer = (ManagedObject *)((byte *)pointer + delta);
}

void LocalMemoryManager::mark() {
    for (Pointer<ManagedObject> *p = pointers; p; p = p->next)
        if (*p && !(*p)->isMarked())
            mark(*p);
}

void LocalMemoryManager::compact() {
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

void LocalMemoryManager::release() {
    std::cout << "\nMemoryManager::release()\n";

    byte *p = memory.getData();

    for (int i = 0, size = 0; i < objectCount; i++, p += size) {
        size = ((ManagedObject *)p)->getSize();
        ((ManagedObject *)p)->~ManagedObject();
    }
}

void LocalMemoryManager::shiftPointers(ManagedObject *object) {
    object->mapOnReferences([this](ManagedObject *&ref) {
        shiftPointer(ref);
    });
}

void LocalMemoryManager::forwardPointers(ManagedObject *object) {
    object->mapOnReferences([](ManagedObject *&ref) {
        ref = ref->forwardAddress;
    });
}

void LocalMemoryManager::mark(ManagedObject *object) {
    object->mark();

    object->mapOnReferences([this](ManagedObject *&ref) {
        if (!ref->isMarked())
            mark(ref);
    });
}
