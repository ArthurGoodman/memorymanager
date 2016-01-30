#include "frame.h"

#include "memorymanager.h"

Frame::Frame(const std::function<void(const std::function<void(ManagedObject *&)> &)> &map)
    : map(map) {
    MemoryManager::pushFrame(this);
}

Frame::~Frame() {
    MemoryManager::popFrame();
}

void Frame::mapOnLocals(const std::function<void(ManagedObject *&)> &f) {
    map(f);
}
