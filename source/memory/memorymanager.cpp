#include "memorymanager.h"

#include "pointer.h"
#include "semispacememorymanager.h"

Pointer<ManagedObject> *&MemoryManager::pointers() {
    static Pointer<ManagedObject> *pointers = 0;
    return pointers;
}

Frame *&MemoryManager::frames() {
    static Frame *frames = 0;
    return frames;
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

void MemoryManager::registerFrame(Frame *frame) {
    frame->link(frames());
}

void MemoryManager::removeFrame(Frame *frame) {
    frame->unlink(frames());
}

MemoryManager::~MemoryManager() {
}
