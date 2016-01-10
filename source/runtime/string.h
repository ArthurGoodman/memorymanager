#pragma once

#include "object.h"

class String : public Object {
    static uint nextId;

    std::string value;

public:
    static uint id(const std::string &str);
    static String *string(uint id);

    String(const std::string &value = "");

    bool equals(Object *object) const;
    ulong hash() const;

    std::string toString() const;

    int getSize() const;

private:
    static Pointer<Map<uint, Object *>> &idString();
    static Pointer<Map<Object *, uint>> &stringId();
};
