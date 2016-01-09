#include "memorymanager.h"

#include "pointer.h"
#include "semispacememorymanager.h"

MemoryManager *MemoryManager::manager;
Pointer<ManagedObject> *MemoryManager::pointers = 0;

void MemoryManager::initialize() {
    manager = new SemispaceMemoryManager;
}

void MemoryManager::finalize() {
    delete manager;
}

void MemoryManager::registerPointer(Pointer<ManagedObject> *pointer) {
    pointer->link(pointers);
}

void MemoryManager::removePointer(Pointer<ManagedObject> *pointer) {
    pointer->unlink(pointers);
}

MemoryManager::~MemoryManager() {
}
