#include "runtime.h"

#include "class.h"
#include "sherwoodmap.h"
#include "pointer.h"
#include "string.h"
#include "true.h"
#include "false.h"
#include "null.h"
#include "nativefunction.h"

void Runtime::initialize() {
    objectClass() = new Class("Object", 0);
    setRootClass(objectClass());

    Pointer<Class> classClass = new Class("Class");
    setRootClass(classClass);

    objectClass()->setClass(classClass);
    classClass->setClass(classClass);

    Pointer<Class> functionClass = new Class("Function");
    setRootClass(functionClass);

    mainObject() = new Object;

    trueObject() = new True;
    falseObject() = new False;
    nullObject() = new Null;

    objectClass()->setAttribute("class", new NativeFunction("class", [](Object *self, const std::list<Object *> &) -> Object * {
                                    return self->getClass();
                                }));
}

Object *Runtime::getMainObject() {
    return mainObject();
}

Class *Runtime::getObjectClass() {
    return objectClass();
}

Object *Runtime::getTrue() {
    return trueObject();
}

Object *Runtime::getFalse() {
    return falseObject();
}

Object *Runtime::getNull() {
    return nullObject();
}

bool Runtime::hasRootClass(const std::string &name) {
    return rootClasses()->contains(String::id(name));
}

Class *Runtime::getRootClass(const std::string &name) {
    uint id = String::id(name);
    return rootClasses()->contains(id) ? (Class *)(rootClasses())->get(id) : 0;
}

void Runtime::setRootClass(Class *rootClass) {
    rootClasses()->put(String::id(rootClass->getName()), rootClass);
}

Object *Runtime::toBoolean(bool value) {
    return value ? trueObject() : falseObject();
}

void Runtime::runtimeError(const std::string &message) {
    throw message;
}

Pointer<Object> &Runtime::mainObject() {
    static Pointer<Object> mainObject;
    return mainObject;
}

Pointer<Class> &Runtime::objectClass() {
    static Pointer<Class> objectClass;
    return objectClass;
}

Pointer<Object> &Runtime::trueObject() {
    static Pointer<Object> trueObject;
    return trueObject;
}

Pointer<Object> &Runtime::falseObject() {
    static Pointer<Object> falseObject;
    return falseObject;
}

Pointer<Object> &Runtime::nullObject() {
    static Pointer<Object> nullObject;
    return nullObject;
}

Pointer<Map<uint, Object *>> &Runtime::rootClasses() {
    static Pointer<Map<uint, Object *>> rootClasses = new SherwoodMap<uint, Object *>;
    return rootClasses;
}
