#include "frame.h"

#include "memorymanager.h"

Frame::Frame(const std::function<void(const std::function<void(ManagedObject *&)> &)> &map)
    : map(map), prev(0), next(0) {
    MemoryManager::registerFrame(this);
}

Frame::~Frame() {
    MemoryManager::removeFrame(this);
}

void Frame::mapOnLocals(const std::function<void(ManagedObject *&)> &f) {
    map(f);
}

void Frame::link(Frame *&frames) {
    next = frames;

    if (frames)
        frames->prev = this;

    frames = this;
}

void Frame::unlink(Frame *&frames) {
    if (next)
        next->prev = prev;

    if (prev)
        prev->next = next;

    if (frames == this)
        frames = frames->next;
}

Frame *Frame::getNext() {
    return next;
}
