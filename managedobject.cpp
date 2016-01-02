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

void ManagedObject::setFlag(int flag, bool value) {
    value ? flags |= flag : flags &= ~flag;
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

byte *ManagedObject::getForwardAddress() {
    return forwardAddress;
}

void ManagedObject::getReferences(References &) {
}
