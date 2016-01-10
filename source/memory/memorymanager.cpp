#include "memorymanager.h"

#include "pointer.h"
#include "semispacememorymanager.h"

Pointer<ManagedObject> *&MemoryManager::pointers() {
    static Pointer<ManagedObject> *pointers = 0;
    return pointers;
}

MemoryManager *MemoryManager::instance() {
    static SemispaceMemoryManager instance;
    return &instance;
}

void MemoryManager::registerPointer(Pointer<ManagedObject> *pointer) {
    pointer->link(pointers());
}

void MemoryManager::removePointer(Pointer<ManagedObject> *pointer) {
    pointer->unlink(pointers());
}

MemoryManager::~MemoryManager() {
}
