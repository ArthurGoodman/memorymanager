#include "object.h"

#include "hashmap.h"
#include "pointer.h"
#include "utility.h"
#include "string.h"

Object::Object()
    : attributes(0) {
}

bool Object::hasAttribute(const std::string &name) {
    return hasAttribute(String::id(name));
}

Object *Object::getAttribute(const std::string &name) {
    return getAttribute(String::id(name));
}

void Object::setAttribute(const std::string &name, Object *value) {
    setAttribute(String::id(name), value);
}

void Object::removeAttribute(const std::string &name) {
    removeAttribute(String::id(name));
}

bool Object::hasAttribute(uint id) {
    return attributes && attributes->contains(id);
}

Object *Object::getAttribute(uint id) {
    return attributes ? attributes->get(id) : 0;
}

void Object::setAttribute(uint id, const Pointer<Object> &value) {
    Pointer<Object> _this = this;

    if (attributes == 0)
        _this->attributes = new HashMap<uint, Object *>;

    _this->attributes->put(id, value);
}

void Object::removeAttribute(uint id) {
    if (attributes)
        attributes->remove(id);
}

bool Object::equals(Object *object) const {
    return this == object;
}

ulong Object::hash() const {
    return (ulong) this;
}

std::string Object::toString() const {
    return Utility::toString((void *)this);
}

void Object::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (attributes)
        f((ManagedObject *&)attributes);
}

int Object::getSize() const {
    return sizeof *this;
}

int Object::attributesCount() const {
    return attributes->size();
}