#include "managedobject.h"

#include "memorymanager.h"

void *ManagedObject::operator new(uint size) {
    return (void *)MemoryManager::instance()->allocate(size);
}

ManagedObject::ManagedObject()
    : flags(0), forwardAddress(0) {
}

ManagedObject::~ManagedObject() {
}

bool ManagedObject::isMarked() {
    return flags & FlagMark;
}

void ManagedObject::unmark() {
    setFlag(FlagMark, false);
}

void ManagedObject::shiftPointers(int) {
}

void ManagedObject::mark() {
    setFlag(FlagMark);
}

void ManagedObject::setFlag(Flag flag, bool value) {
    value ? flags |= flag : flags &= ~flag;
}
