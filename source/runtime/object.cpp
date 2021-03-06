#include "object.h"

#include "class.h"
#include "function.h"
#include "hashmap.h"
#include "utility.h"
#include "mystring.h"
#include "runtime.h"
#include "memorymanager.h"

Object::Object()
    : objectClass(Runtime::getObjectClass()), attributes(0) {
}

Object::Object(Class *objectClass)
    : objectClass(objectClass), attributes(0) {
}

Object::Object(std::string className)
    : objectClass((Class *)Runtime::getRootClass(className)), attributes(0) {
}

Class *Object::getClass() {
    return objectClass;
}

void Object::setClass(Class *objectClass) {
    this->objectClass = objectClass;
}

bool Object::hasAttribute(const std::string &name) const {
    return hasAttribute(String::id(name));
}

Object *Object::getAttribute(const std::string &name) const {
    return getAttribute(String::id(name));
}

void Object::setAttribute(const std::string &name, Object *value) {
    setAttribute(String::id(name), value);
}

void Object::removeAttribute(const std::string &name) {
    removeAttribute(String::id(name));
}

bool Object::hasAttribute(uint id) const {
    return attributes && attributes->contains(id);
}

Object *Object::getAttribute(uint id) const {
    return attributes ? attributes->get(id) : 0;
}

void Object::setAttribute(uint id, Object *value) {
    Object *_this = this;

    GC_FRAME(POINTER(value) POINTER(_this))

    if (attributes == 0)
        _this->attributes = new HashMap<uint, Object *>;

    _this->attributes->put(id, value);
}

void Object::removeAttribute(uint id) {
    if (attributes)
        attributes->remove(id);
}

int Object::attributesCount() const {
    return attributes->size();
}

bool Object::instanceOf(Class *_class) {
    return objectClass->isChild(_class);
}

Object *Object::call(const std::string &name, const std::list<Object *> &args) {
    Function *function = dynamic_cast<Function *>(objectClass->lookup(name));

    if (!function)
        Runtime::runtimeError("call error");

    return function->invoke(this, args);
}

Object *Object::call(const std::string &name, Object *arg) {
    return call(name, std::list<Object *>(1, arg));
}

Object *Object::call(const std::string &name) {
    return call(name, std::list<Object *>());
}

bool Object::isTrue() const {
    return true;
}

bool Object::isFalse() const {
    return false;
}

bool Object::isNull() const {
    return false;
}

bool Object::equals(Object *object) const {
    return this == object;
}

std::string Object::toString() const {
    return Utility::toString((void *)this);
}

ulong Object::hash() const {
    return (ulong) this;
}

void Object::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    if (objectClass)
        f((ManagedObject *&)objectClass);

    if (attributes)
        f((ManagedObject *&)attributes);
}

int Object::getSize() const {
    return sizeof *this;
}
