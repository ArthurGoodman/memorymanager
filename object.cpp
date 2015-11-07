#include "object.h"

#include "memorymanager.h"

Object::Object()
    : flags(0), attributes(0) {
}

void Object::setFlag(Flag flag, bool value) {
    value ? flags |= flag : flags &= ~flag;
}

void *Object::operator new(uint size) {
    return (void *)MemoryManager::instance()->allocate(size);
}

void Object::operator delete(void *p) {
    return MemoryManager::instance()->free((Object *)p);
}
