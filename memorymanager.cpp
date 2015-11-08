#include "memorymanager.h"

#include "object.h"

MemoryManager *MemoryManager::manager = new MemoryManager;

Object *MemoryManager::allocate(int size) {
    return (Object *)memory.allocate(size);
}

void MemoryManager::free(Object *object) {
    object->setFlag(Object::FlagFree);
}

MemoryManager::MemoryManager() {
}
