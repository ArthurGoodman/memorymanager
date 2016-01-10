#include "runtime.h"

#include "class.h"
#include "sherwoodmap.h"
#include "pointer.h"
#include "string.h"
#include "true.h"
#include "false.h"
#include "null.h"
#include "nativefunction.h"

Pointer<Object> *Runtime::mainObject;
Pointer<Class> *Runtime::objectClass;

Pointer<Object> *Runtime::trueObject;
Pointer<Object> *Runtime::falseObject;
Pointer<Object> *Runtime::nullObject;

Pointer<SherwoodMap<uint, Object *>> *Runtime::rootClasses;

void Runtime::initialize() {
    mainObject = new Pointer<Object>;
    objectClass = new Pointer<Class>;

    trueObject = new Pointer<Object>;
    falseObject = new Pointer<Object>;
    nullObject = new Pointer<Object>;

    rootClasses = new Pointer<SherwoodMap<uint, Object *>>(new SherwoodMap<uint, Object *>);

    *objectClass = new Class("Object", 0);
    setRootClass(*objectClass);

    Pointer<Class> classClass = new Class("Class");
    setRootClass(classClass);

    (*objectClass)->setClass(classClass);
    classClass->setClass(classClass);

    Pointer<Class> functionClass = new Class("Function");
    setRootClass(functionClass);

    *mainObject = new Object;

    *trueObject = new True;
    *falseObject = new False;
    *nullObject = new Null;

    auto classFunction = [](Object * self, const std::list<Object *> &) -> Object * {
        return self->getClass();
    };

    (*objectClass)->setAttribute("class", new NativeFunction("class", classFunction));
}

void Runtime::finalize() {
    delete mainObject;
    delete objectClass;

    delete trueObject;
    delete falseObject;
    delete nullObject;

    delete rootClasses;
}

Object *Runtime::getMainObject() {
    return *mainObject;
}

Class *Runtime::getObjectClass() {
    return *objectClass;
}

Object *Runtime::getTrue() {
    return *trueObject;
}

Object *Runtime::getFalse() {
    return *falseObject;
}

Object *Runtime::getNull() {
    return *nullObject;
}

bool Runtime::hasRootClass(const std::string &name) {
    return (*rootClasses)->contains(String::id(name));
}

Class *Runtime::getRootClass(const std::string &name) {
    uint id = String::id(name);
    return (*rootClasses)->contains(id) ? (Class *)(*rootClasses)->get(id) : 0;
}

void Runtime::setRootClass(Class *rootClass) {
    (*rootClasses)->put(String::id(rootClass->getName()), rootClass);
}

Object *Runtime::toBoolean(bool value) {
    return value ? *trueObject : *falseObject;
}

void Runtime::runtimeError(const std::string &message) {
    throw message;
}
