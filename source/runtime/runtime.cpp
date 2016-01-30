#include "runtime.h"

#include "class.h"
#include "sherwoodmap.h"
#include "string.h"
#include "true.h"
#include "false.h"
#include "null.h"
#include "nativefunction.h"
#include "memorymanager.h"

Object *Runtime::mainObject = 0;
Class *Runtime::objectClass = 0;

Object *Runtime::trueObject = 0;
Object *Runtime::falseObject = 0;
Object *Runtime::nullObject = 0;

Map<uint, Object *> *Runtime::rootClasses = 0;

void Runtime::initialize() {
    Class *classClass = 0, *functionClass = 0;

    GC_OPEN_STATIC_FRAME
    POINTER(mainObject)
    POINTER(objectClass)
    POINTER(trueObject)
    POINTER(falseObject)
    POINTER(nullObject)
    POINTER(rootClasses)
    POINTER(classClass)
    POINTER(functionClass)
    GC_CLOSE_FRAME

    rootClasses = new SherwoodMap<uint, Object *>;

    objectClass = new Class("Object", 0);
    setRootClass(objectClass);

    classClass = new Class("Class");
    setRootClass(classClass);

    objectClass->setClass(classClass);
    classClass->setClass(classClass);

    functionClass = new Class("Function");
    setRootClass(functionClass);

    mainObject = new Object;

    trueObject = new True;
    falseObject = new False;
    nullObject = new Null;

    objectClass->setAttribute("class", new NativeFunction("class", [](Object *self, const std::list<Object *> &) -> Object * {
                                  return self->getClass();
                              }));
}

Object *Runtime::getMainObject() {
    return mainObject;
}

Class *Runtime::getObjectClass() {
    return objectClass;
}

Object *Runtime::getTrue() {
    return trueObject;
}

Object *Runtime::getFalse() {
    return falseObject;
}

Object *Runtime::getNull() {
    return nullObject;
}

bool Runtime::hasRootClass(const std::string &name) {
    return rootClasses->contains(String::id(name));
}

Class *Runtime::getRootClass(const std::string &name) {
    uint id = String::id(name);
    return rootClasses->contains(id) ? (Class *)(rootClasses)->get(id) : 0;
}

void Runtime::setRootClass(Class *rootClass) {
    rootClasses->put(String::id(rootClass->getName()), rootClass);
}

Object *Runtime::toBoolean(bool value) {
    return value ? trueObject : falseObject;
}

void Runtime::runtimeError(const std::string &message) {
    throw message;
}
