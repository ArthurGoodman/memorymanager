#include "memorymanager.h"

#include "object.h"

MemoryManager *MemoryManager::manager = new MemoryManager;

Object *MemoryManager::allocate(int size) {
    byte *oldAddress = memory.getData();
    Object *object = (Object *)memory.allocate(size);
    delta = memory.getData() - oldAddress;

    objectCount++;

//    if (delta > 0)
        shiftPointers();

    return object;
}

void MemoryManager::free(Object *object) {
    object->setFlag(Object::FlagFree);
}

int MemoryManager::getDelta() {
    return delta;
}

MemoryManager::MemoryManager()
    : delta(0), objectCount(0) {
}

void MemoryManager::shiftPointers() {
    Object *objects = (Object *)memory.getData();

    for (int i = 0; i < objectCount - 1; i++) {
        objects->shiftPointers(delta);
        objects += objects->getSize();
    }
}
