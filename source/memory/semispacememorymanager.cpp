#include "semispacememorymanager.h"

#include <utility>

#include "pointer.h"
#include "managedobject.h"

#include <iostream>

SemispaceMemoryManager::SemispaceMemoryManager()
    : objectCount(0), memoryUsed(0), capacity(InitialCapacity), delta(0) {
    initialize();
}

SemispaceMemoryManager::~SemispaceMemoryManager() {
    finalize();
}

ManagedObject *SemispaceMemoryManager::allocate(uint size, int count) {
    if (!enoughSpace(size))
        collectGarbage();

    if (!enoughSpace(size))
        expand();
    else
        delta = 0;

    ManagedObject *object = (ManagedObject *)allocPtr;
    allocPtr += size;

    objectCount += count;
    memoryUsed += size;

    return object;
}

void SemispaceMemoryManager::free(ManagedObject *) {
}

void SemispaceMemoryManager::collectGarbage() {
    std::cout << "\nSemispaceMemoryManager::collectGarbage()\n";

    int oldMemoryUsed = memoryUsed, oldObjectCount = objectCount;

    std::swap(fromSpace, toSpace);
    allocPtr = toSpace;

    objectCount = 0;
    memoryUsed = 0;

    for (Pointer<ManagedObject> *p = pointers(); p; p = p->getNext())
        if (*p)
            *p = copy((ManagedObject *)((byte *)**p + delta));

    for (Frame *frame : frames())
        frame->mapOnLocals([this](ManagedObject *&p) {
            p = copy((ManagedObject *)((byte *)p + delta));
        });

    std::cout << "//freed=" << oldMemoryUsed - memoryUsed << ", freedObjects=" << oldObjectCount - objectCount << ", objectCount=" << objectCount << "\n\n";
}

void SemispaceMemoryManager::initialize() {
    toSpace = memory.allocate(capacity);
    fromSpace = toSpace + capacity / 2;
    allocPtr = toSpace;
}

void SemispaceMemoryManager::finalize() {
    std::cout << "Memory used: " << memoryUsed << "\n";
    std::cout << "Total memory: " << memory.getSize() << "\n";
    std::cout << "\nSemispaceMemoryManager::finalize()\n";
}

bool SemispaceMemoryManager::enoughSpace(int size) const {
    return allocPtr + size <= toSpace + capacity / 2;
}

ManagedObject *SemispaceMemoryManager::copy(ManagedObject *object) {
    if (object->getForwardAddress() == 0) {
        objectCount++;
        memoryUsed += object->getSize();

        ManagedObject *newObject = (ManagedObject *)allocPtr;
        allocPtr += object->getSize();

        memcpy(newObject, object, object->getSize());

        object->setForwardAddress(newObject);

        newObject->mapOnReferences([this](ManagedObject *&ref) {
            ref = copy((ManagedObject *)((byte *)ref + delta));
        });
    }

    return object->getForwardAddress();
}

void SemispaceMemoryManager::expand() {
    std::cout << "SemispaceMemoryManager::expand()\n";

    byte *oldData = memory.getData();

    fromSpace = memory.allocate(capacity);
    toSpace = fromSpace - capacity;
    capacity *= 2;

    delta = memory.getData() - oldData;

    std::cout << "//delta=" << delta << "\n";

    collectGarbage();
}
