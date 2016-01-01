#include "managedobject.h"

#include "memorymanager.h"

void *ManagedObject::operator new(uint size) {
    return (void *)MemoryManager::instance()->allocate(size);
}

ManagedObject::ManagedObject()
    : flags(0) {
}

ManagedObject::~ManagedObject() {
}

void ManagedObject::setFlag(Flag flag, bool value) {
    value ? flags |= flag : flags &= ~flag;
}

bool ManagedObject::isFree() {
    return flags & FlagFree;
}

void ManagedObject::shiftPointers(int) {
}

void ManagedObject::mark() {
    setFlag(FlagMark);
}
