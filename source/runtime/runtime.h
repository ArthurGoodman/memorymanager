#pragma once

#include <string>

#include "common.h"
#include "pointer.h"
#include "map.h"
#include "object.h"
#include "class.h"

class Runtime {
public:
    static void initialize();

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

protected:
    static Pointer<Object> &mainObject();
    static Pointer<Class> &objectClass();

    static Pointer<Object> &trueObject();
    static Pointer<Object> &falseObject();
    static Pointer<Object> &nullObject();

    static Pointer<Map<uint, Object *>> &rootClasses();
};
