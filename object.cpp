#include "object.h"

#include "hashtable.h"
#include "utility.h"

Object::Object()
    : attributes(0) {
}

void Object::shiftPointers(int delta) {
    if (attributes)
        MemoryManager::shiftPointer(attributes, delta);
}

void Object::mark() {
    ManagedObject::mark();

    if (attributes)
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

void Object::setAttribute(std::string name, Pointer<Object> value) {
    Pointer<Object> _this = this;

    if (!attributes)
        attributes = new HashTable<std::string, Object *>;

    _this->attributes->put(name, value);
}

std::string Object::toString() {
    return Utility::toString<void *>(this);
}
