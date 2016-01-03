#include "nomemorymanager.h"

#include <memory>

ManagedObject *NoMemoryManager::allocate(uint size) {
    return (ManagedObject *)malloc(size);
}

void NoMemoryManager::free(ManagedObject *p) {
    ::free(p);
}

void NoMemoryManager::collectGarbage() {
}

void NoMemoryManager::registerPointer(Pointer<ManagedObject> *) {
}

void NoMemoryManager::removePointer(Pointer<ManagedObject> *) {
}
