#include "managedobject.h"

#include "imemorymanager.h"

void *ManagedObject::operator new(uint size) {
    return (void *)IMemoryManager::instance()->allocate(size);
}

void ManagedObject::operator delete(void *p) {
    IMemoryManager::instance()->free((ManagedObject *)p);
}

ManagedObject::ManagedObject()
    : flags(0), forwardAddress(0) {
}

ManagedObject::~ManagedObject() {
}

void ManagedObject::mapOnReferences(const std::function<void(ManagedObject *&)> &) {
}
