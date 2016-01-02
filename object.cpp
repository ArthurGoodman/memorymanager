#include "object.h"

#include "hashtable.h"
#include "pointer.h"
#include "utility.h"
#include "memorymanager.h"

Object::Object()
    : attributes(0) {
}

void Object::shiftPointers() {
    if (attributes)
        MemoryManager::shiftPointer(attributes);
}

void Object::forwardPointers() {
    if (attributes)
        attributes = (HashTable *)attributes->forwardAddress;
}

void Object::mark() {
    ManagedObject::mark();

    if (attributes && !attributes->isMarked())
        attributes->mark();
}

int Object::getSize() {
    return sizeof *this;
}

bool Object::hasAttribute(std::string name) {
    return attributes && attributes->contains(name);
}

Object *Object::getAttribute(std::string name) {
    return attributes ? attributes->get(name) : 0;
}

void Object::setAttribute(std::string name, const Pointer<Object> &value) {
    Pointer<Object> _this = this;

    if (!attributes)
        _this->attributes = new HashTable;

    _this->attributes->put(name, value);
}

std::string Object::toString() {
    return Utility::toString<void *>(this);
}
