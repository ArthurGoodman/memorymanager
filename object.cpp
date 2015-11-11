#include "object.h"

#include "memorymanager.h"
#include "hashtable.h"

void *Object::operator new(uint size) {
    return (void *)MemoryManager::instance()->allocate(size);
}

void Object::operator delete(void *p) {
    return MemoryManager::instance()->free((Object *)p);
}

Object::Object()
    : flags(0), attributes(0) {
}

Object::~Object() {
}

void Object::setFlag(Flag flag, bool value) {
    value ? flags |= flag : flags &= ~flag;
}

void Object::mark() {
    setFlag(FlagMark);

    if (attributes)
        attributes->mark();
}

ulong Object::hash() {
    ulong value = (ulong) this;

    value = ~value + (value << 15);
    value = value ^ (value >> 12);
    value = value + (value << 2);
    value = value ^ (value >> 4);
    value = value * 2057;
    value = value ^ (value >> 16);

    return value;
}

void Object::shiftPointers(int delta) {
    if (attributes) {
        attributes += delta;
        attributes->shiftPointers(delta);
    }
}

int Object::getSize() {
    return sizeof(*this);
}
