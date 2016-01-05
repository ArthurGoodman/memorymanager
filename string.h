#pragma once

#include "object.h"

class String : public Object {
    static Pointer<Map<Object *, uint>> *stringId;
    static Pointer<Map<uint, Object *>> *idString;
    static uint lastId;

    std::string value;

public:
    static void initialize();
    static void finalize();

    static uint stringToId(const std::string &str);
    static String *idToString(uint id);

    String(const std::string &value = "");

    bool equals(Object *object) const;
    ulong hash() const;

    std::string toString() const;

    int getSize() const;
};
