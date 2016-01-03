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
    finalize();
}

ManagedObject *LocalMemoryManager::allocate(uint size) {
    if (!memory.enoughSpace(size))
        collectGarbage();

    byte *oldAddress = memory.getData();

    ManagedObject *object = (ManagedObject *)memory.allocate(size);

    if ((delta = memory.getData() - oldAddress) != 0)
        shiftPointers();

    objectCount++;

    return object;
}

void LocalMemoryManager::free(ManagedObject *) {
}

void LocalMemoryManager::collectGarbage() {
    std::cout << "\nMemoryManager::collectGarbage()\n";

    int oldSize = memory.getSize(), oldObjectCount = objectCount;

    mark();
    compact();

    std::cout << "//freed=" << oldSize - memory.getSize() << ", freedObjects=" << oldObjectCount - objectCount << ", objectCount=" << objectCount << "\n\n";
}

void LocalMemoryManager::registerPointer(Pointer<ManagedObject> *p) {
    p->link(pointers);
}

void LocalMemoryManager::removePointer(Pointer<ManagedObject> *p) {
    p->unlink(pointers);
}

void LocalMemoryManager::shiftPointers() {
    std::cout << "MemoryManager::shiftPointers() //delta=" << delta << "\n\n";

    byte *object = memory.getData();

    for (int i = 0; i < objectCount; i++, object += ((ManagedObject *)object)->getSize())
        shiftPointers((ManagedObject *)object);

    for (Pointer<ManagedObject> *p = pointers; p; p = p->getNext())
        if (*p)
            shiftPointer(**p);
}

void LocalMemoryManager::shiftPointer(ManagedObject *&pointer) {
    pointer = (ManagedObject *)((byte *)pointer + delta);
}

void LocalMemoryManager::mark() {
    for (Pointer<ManagedObject> *p = pointers; p; p = p->getNext())
        if (*p && !(*p)->hasFlag(ManagedObject::FlagMark))
            mark(*p);
}

void LocalMemoryManager::compact() {
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

    for (Pointer<ManagedObject> *p = pointers; p; p = p->getNext())
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

void LocalMemoryManager::finalize() {
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
        ref = ref->getForwardAddress();
    });
}

void LocalMemoryManager::mark(ManagedObject *object) {
    object->setFlag(ManagedObject::FlagMark);

    object->mapOnReferences([this](ManagedObject *&ref) {
        if (!ref->hasFlag(ManagedObject::FlagMark))
            mark(ref);
    });
}
