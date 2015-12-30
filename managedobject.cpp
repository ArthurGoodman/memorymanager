#include "managedobject.h"

#include "memorymanager.h"

void *ManagedObject::operator new(uint size) {
    return (void *)MemoryManager::instance()->allocate(size);
}

void ManagedObject::operator delete(void *p) {
    return MemoryManager::instance()->free((ManagedObject *)p);
}

ManagedObject::ManagedObject()
    : flags(0) {
}

ManagedObject::~ManagedObject() {
}

void ManagedObject::setFlag(Flag flag, bool value) {
    value ? flags |= flag : flags &= ~flag;
}

void ManagedObject::shiftPointers(int) {
}

void ManagedObject::shiftPointersAgain(int) {
}

void ManagedObject::mark() {
    setFlag(FlagMark);
}
