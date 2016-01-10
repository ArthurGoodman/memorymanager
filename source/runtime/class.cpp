#include "class.h"

#include "runtime.h"

Class::Class(const std::string &name, Class *superclass)
    : Object("Class"), name(name), superclass(superclass) {
}

Class::Class(const std::string &name, const std::string &superclassName)
    : Object("Class"), name(name), superclass((Class *)Runtime::getRootClass(superclassName)) {
}

Class::Class(const std::string &name)
    : Object("Class"), name(name), superclass(Runtime::getObjectClass()) {
}

std::string Class::getName() {
    return name;
}

Class *Class::getSuperclass() {
    return superclass;
}

void Class::setSuperclass(Class *superclass) {
    this->superclass = superclass;
}

bool Class::isChild(Class *_class) {
    return this == _class || (superclass ? (superclass == _class ? true : superclass->isChild(_class)) : false);
}

Object *Class::newInstance() {
    return createNewInstance();
}

Object *Class::lookup(std::string name) {
    if (hasAttribute(name))
        return getAttribute(name);

    if (superclass)
        return superclass->lookup(name);

    return 0;
}

std::string Class::toString() {
    return name.empty() ? "<anonymous class>" : name;
}

void Class::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    f((ManagedObject *&)superclass);
}

int Class::getSize() const {
    return sizeof *this;
}

Object *Class::createNewInstance() {
    return new Object(this);
}
