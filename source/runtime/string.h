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
    static Map<uint, Object *> *idString();
    static Map<Object *, uint> *stringId();
};
