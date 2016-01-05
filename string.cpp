#include "string.h"

#include "sherwoodmap.h"
#include "pointer.h"

#include <iostream>

Pointer<Map<Object *, uint>> *String::stringId;
Pointer<Map<uint, Object *>> *String::idString;
uint String::lastId = 0;

void String::initialize() {
    stringId = new Pointer<Map<Object *, uint>>(new SherwoodMap<Object *, uint>);
    idString = new Pointer<Map<uint, Object *>>(new SherwoodMap<uint, Object *>);
}

void String::finalize() {
    delete stringId;
    delete idString;
}

uint String::stringToId(const std::string &str) {
    String fakeString(str);

    if ((*stringId)->contains(&fakeString))
        return (*stringId)->get(&fakeString);

    Pointer<Object> string = new String(str);
    uint id = lastId++;

    (*stringId)->put(string, id);
    (*idString)->put(id, string);

    return id;
}

String *String::idToString(uint id) {
    return (String *)(*idString)->get(id);
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
