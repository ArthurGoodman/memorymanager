#include "mystring.h"

#include "sherwoodmap.h"
#include "memorymanager.h"
#include <iostream>

uint String::nextId = 0;

uint String::id(const std::string &str) {
    Object *string = 0;

    GC_FRAME(POINTER(string))

    byte data[sizeof(String)];
    String *fakeString = new (data) String(str);

    if (stringId()->contains(fakeString))
        return stringId()->get(fakeString);

    string = new String(str);
    uint id = nextId++;

    stringId()->put(string, id);
    idString()->put(id, string);

    return id;
}

String *String::string(uint id) {
    return (String *)idString()->get(id);
}

String::String(const std::string &value)
    : value(value) {
    std::cout << "String::String(value=" << value << ")\n";
}

bool String::equals(Object *object) const {
    return dynamic_cast<String *>(object) && ((String *)object)->value == value;
}

ulong String::hash() const {
    return std::hash<std::string>()(value);
}

std::string String::toString() const {
    return value;
}

int String::getSize() const {
    return sizeof *this;
}

inline Map<uint, Object *> *String::idString() {
    static Map<uint, Object *> *idString = new SherwoodMap<uint, Object *>;
    GC_STATIC_FRAME(POINTER(idString))
    return idString;
}

inline Map<Object *, uint> *String::stringId() {
    static Map<Object *, uint> *stringId = new SherwoodMap<Object *, uint>;
    GC_STATIC_FRAME(POINTER(stringId))
    return stringId;
}
