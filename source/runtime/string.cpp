#include "string.h"

#include "sherwoodmap.h"
#include "pointer.h"

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

inline Pointer<Map<uint, Object *>> &String::idString() {
    static Pointer<Map<uint, Object *>> idString = new SherwoodMap<uint, Object *>;
    return idString;
}

inline Pointer<Map<Object *, uint>> &String::stringId() {
    static Pointer<Map<Object *, uint>> stringId = new SherwoodMap<Object *, uint>;
    return stringId;
}
