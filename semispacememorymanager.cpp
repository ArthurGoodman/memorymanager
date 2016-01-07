#include "semispacememorymanager.h"

#include <utility>

#include "pointer.h"
#include "managedobject.h"

#include <iostream>

SemispaceMemoryManager::SemispaceMemoryManager()
    : pointers(0), objectCount(0), capacity(InitialCapacity) {
    toSpace = memory.allocate(capacity);
    fromSpace = toSpace + capacity / 2;

    allocPtr = toSpace;
}

ManagedObject *SemispaceMemoryManager::allocate(uint size, int /*count*/) {
    if (!enoughSpace(size))
        collectGarbage();

    if (!enoughSpace(size)) {
        expand();

        std::cout << "error: not enough space\n";
        return 0;
    }

    ManagedObject *object = (ManagedObject *)allocPtr;
    allocPtr += size;

    return object;
}

void SemispaceMemoryManager::free(ManagedObject *) {
}

void SemispaceMemoryManager::collectGarbage() {
    std::cout << "\nSemispaceMemoryManager::collectGarbage()\n";

    std::swap(fromSpace, toSpace);
    allocPtr = toSpace;

    for (Pointer<ManagedObject> *p = pointers; p; p = p->getNext())
        if (*p)
            *p = copy(*p);
}

void SemispaceMemoryManager::registerPointer(Pointer<ManagedObject> *pointer) {
    pointer->link(pointers);
}

void SemispaceMemoryManager::removePointer(Pointer<ManagedObject> *pointer) {
    pointer->unlink(pointers);
}

bool SemispaceMemoryManager::enoughSpace(int size) const {
    return allocPtr + size <= toSpace + capacity / 2;
}

ManagedObject *SemispaceMemoryManager::copy(ManagedObject *object) {
    if (object->getForwardAddress() == 0) {
        ManagedObject *newObject = (ManagedObject *)allocPtr;
        allocPtr += object->getSize();

        memcpy(newObject, object, object->getSize());

        object->setForwardAddress(newObject);

        newObject->mapOnReferences([this](ManagedObject *&ref) {
            ref = copy(ref);
        });
    }

    return object->getForwardAddress();
}

void SemispaceMemoryManager::expand() {
    std::cout << "SemispaceMemoryManager::expand()\n";
}
