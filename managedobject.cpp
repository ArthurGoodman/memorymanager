#include "managedobject.h"

#include "imemorymanager.h"

void *ManagedObject::operator new(uint size) {
    return (void *)IMemoryManager::instance()->allocate(size);
}

ManagedObject::ManagedObject()
    : flags(0), forwardAddress(0) {
}

ManagedObject::~ManagedObject() {
}

void ManagedObject::setFlag(int flag, bool value) {
    value ? flags |= flag : flags &= ~flag;
}

void ManagedObject::mapOnReferences(void (*)(ManagedObject *&)) {
}

bool ManagedObject::isMarked() {
    return flags & FlagMark;
}

void ManagedObject::mark() {
    setFlag(FlagMark);
}

void ManagedObject::unmark() {
    setFlag(FlagMark, false);
}
