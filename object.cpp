#include "object.h"

#include "hashtable.h"
#include "pointer.h"
#include "utility.h"
#include "imemorymanager.h"

Object::Object()
    : attributes(0) {
}

bool Object::hasAttribute(const std::string &name) {
    return attributes && attributes->contains(name);
}

Object *Object::getAttribute(const std::string &name) {
    return attributes ? attributes->get(name) : 0;
}

void Object::setAttribute(const std::string &name, const Pointer<Object> &value) {
    Pointer<Object> _this = this;

    if (!attributes)
        _this->attributes = new HashTable;

    _this->attributes->put(name, value);
}

void Object::removeAttribute(const std::string &name) {
    if (attributes)
        attributes->remove(name);
}

std::string Object::toString() {
    return Utility::toString<void *>(this);
}

void Object::mapOnReferences(void (*f)(ManagedObject *&)) {
    if (attributes)
        f((ManagedObject *&)attributes);
}

int Object::getSize() {
    return sizeof *this;
}
