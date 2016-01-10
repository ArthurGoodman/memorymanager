#pragma once

#include <string>

#include "common.h"

class Object;
class Class;

template<class>
class Pointer;

template<class, class>
class SherwoodMap;

class Runtime {
    static Pointer<Object> *mainObject;
    static Pointer<Class> *objectClass;

    static Pointer<Object> *trueObject;
    static Pointer<Object> *falseObject;
    static Pointer<Object> *nullObject;

    static Pointer<SherwoodMap<uint, Object *>> *rootClasses;

public:
    static void initialize();
    static void finalize();

    static Object *getMainObject();
    static Class *getObjectClass();
    static Object *getTrue();
    static Object *getFalse();
    static Object *getNull();

    static bool hasRootClass(const std::string &name);
    static Class *getRootClass(const std::string &name);
    static void setRootClass(Class *rootClass);

    static Object *toBoolean(bool value);

    static void runtimeError(const std::string &message);
};
