#include "object.h"

#include "hashtable.h"
#include "pointer.h"
#include "utility.h"
#include "imemorymanager.h"
#include "string.h"

Object::Object()
    : attributes(0) {
}

bool Object::hasAttribute(const std::string &name) {
    return attributes && attributes->contains(String::stringToId(name));
}

Object *Object::getAttribute(const std::string &name) {
    return attributes ? attributes->get(String::stringToId(name)) : 0;
}

void Object::setAttribute(const std::string &name, const Pointer<Object> &value) {
    Pointer<Object> _this = this;

    if (!attributes)
        _this->attributes = new HashTable<uint, Object *>;

    _this->attributes->put(String::stringToId(name), value);
}

void Object::removeAttribute(const std::string &name) {
    if (attributes)
        attributes->remove(String::stringToId(name));
}

bool Object::equals(Object *object) {
    return this == object;
}

ulong Object::hash() {
    return (ulong) this;
}

std::string Object::toString() {
    return Utility::toString<void *>(this);
}

void Object::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (attributes)
        f((ManagedObject *&)attributes);
}

int Object::getSize() {
    return sizeof *this;
}
