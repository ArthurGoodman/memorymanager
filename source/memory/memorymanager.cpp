#include "memorymanager.h"

#include "pointer.h"
#include "semispacememorymanager.h"

Pointer<ManagedObject> *&MemoryManager::pointers() {
    static Pointer<ManagedObject> *pointers = 0;
    return pointers;
}

std::list<Frame *> &MemoryManager::frames() {
    static std::list<Frame *> frames;
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

void MemoryManager::pushFrame(Frame *frame) {
    frames().push_back(frame);
}

void MemoryManager::popFrame() {
    frames().pop_back();
}

MemoryManager::~MemoryManager() {
}
