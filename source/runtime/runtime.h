#pragma once

#include <string>

#include "common.h"
#include "map.h"
#include "object.h"
#include "class.h"

class Runtime {
private:
    static Object *mainObject;
    static Class *objectClass;

    static Object *trueObject;
    static Object *falseObject;
    static Object *nullObject;

    static Map<uint, Object *> *rootClasses;

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
};
