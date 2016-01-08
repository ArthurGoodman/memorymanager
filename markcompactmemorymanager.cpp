#include "markcompactmemorymanager.h"

#include "managedobject.h"
#include "pointer.h"

#include <iostream>

MarkCompactMemoryManager::MarkCompactMemoryManager()
    : objectCount(0) {
    ByteArray::setInitialCapacity(1024);
}

MarkCompactMemoryManager::~MarkCompactMemoryManager() {
    finalize();
}

ManagedObject *MarkCompactMemoryManager::allocate(uint size, int count) {
    if (!memory.enoughSpace(size))
        collectGarbage();

    ManagedObject *object = (ManagedObject *)memory.allocate(size);

    if (memory.getDelta() != 0)
        updatePointers();

    objectCount += count;

    return object;
}

void MarkCompactMemoryManager::free(ManagedObject *) {
}

void MarkCompactMemoryManager::collectGarbage() {
    std::cout << "\nMarkCompactMemoryManager::collectGarbage()\n";

    int oldSize = memory.getSize(), oldObjectCount = objectCount;

    mark();
    compact();

    std::cout << "//freed=" << oldSize - memory.getSize() << ", freedObjects=" << oldObjectCount - objectCount << ", objectCount=" << objectCount << "\n\n";
}

void MarkCompactMemoryManager::updatePointers() {
    std::cout << "MarkCompactMemoryManager::updatePointers() //delta=" << memory.getDelta() << "\n\n";

    byte *object = memory.getData();

    for (int i = 0; i < objectCount; i++, object += ((ManagedObject *)object)->getSize())
        updatePointers((ManagedObject *)object);

    for (Pointer<ManagedObject> *p = getPointers(); p; p = p->getNext())
        if (*p)
            updatePointer(**p);
}

void MarkCompactMemoryManager::mark() {
    for (Pointer<ManagedObject> *p = getPointers(); p; p = p->getNext())
        if (*p && !(*p)->hasFlag(ManagedObject::FlagMark))
            mark(*p);
}

void MarkCompactMemoryManager::compact() {
    byte *object, *free;
    object = free = memory.getData();

    for (int i = 0; i < objectCount; i++, object += ((ManagedObject *)object)->getSize())
        if (((ManagedObject *)object)->hasFlag(ManagedObject::FlagMark)) {
            ((ManagedObject *)object)->setForwardAddress((ManagedObject *)free);
            free += ((ManagedObject *)object)->getSize();
        }

    object = memory.getData();

    for (int i = 0; i < objectCount; i++, object += ((ManagedObject *)object)->getSize())
        if (((ManagedObject *)object)->hasFlag(ManagedObject::FlagMark))
            forwardPointers((ManagedObject *)object);

    for (Pointer<ManagedObject> *p = getPointers(); p; p = p->getNext())
        if (*p && (*p)->hasFlag(ManagedObject::FlagMark))
            *p = (*p)->getForwardAddress();

    object = memory.getData();

    int freeCount = 0, freeSize = 0;

    for (int i = 0, size = 0; i < objectCount; i++, object += size) {
        size = ((ManagedObject *)object)->getSize();

        if (((ManagedObject *)object)->hasFlag(ManagedObject::FlagMark)) {
            byte *dst = (byte *)((ManagedObject *)object)->getForwardAddress();

            memmove(dst, object, size);

            ((ManagedObject *)dst)->removeFlag(ManagedObject::FlagMark);
            ((ManagedObject *)dst)->setForwardAddress(0);
        } else {
            freeSize += size;
            freeCount++;

            ((ManagedObject *)object)->~ManagedObject();
        }
    }

    memory.free(freeSize);
    objectCount -= freeCount;
}

void MarkCompactMemoryManager::finalize() {
    std::cout << "Memory used: " << memory.getSize() << "\n\nMarkCompactMemoryManager::finalize()\n";

    byte *p = memory.getData();

    for (int i = 0, size = 0; i < objectCount; i++, p += size) {
        size = ((ManagedObject *)p)->getSize();
        ((ManagedObject *)p)->~ManagedObject();
    }
}

void MarkCompactMemoryManager::updatePointers(ManagedObject *object) {
    object->mapOnReferences([this](ManagedObject *&ref) {
        updatePointer(ref);
    });
}

void MarkCompactMemoryManager::forwardPointers(ManagedObject *object) {
    object->mapOnReferences([](ManagedObject *&ref) {
        ref = ref->getForwardAddress();
    });
}

void MarkCompactMemoryManager::mark(ManagedObject *object) {
    object->setFlag(ManagedObject::FlagMark);

    object->mapOnReferences([this](ManagedObject *&ref) {
        if (!ref->hasFlag(ManagedObject::FlagMark))
            mark(ref);
    });
}
