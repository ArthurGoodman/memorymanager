#include "string.h"

#include "hashtable.h"
#include "pointer.h"

#include <iostream>

Pointer<HashTable<Object *, uint>> *String::stringId;
Pointer<HashTable<uint, Object *>> *String::idString;
uint String::lastId = 0;

void String::initialize() {
    stringId = new Pointer<HashTable<Object *, uint>>(new HashTable<Object *, uint>);
    idString = new Pointer<HashTable<uint, Object *>>(new HashTable<uint, Object *>);
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

bool String::equals(Object *object) {
    return dynamic_cast<String *>(object) && ((String *)object)->value == value;
}

ulong String::hash() {
    return std::hash<std::string>()(value);
}

std::string String::toString() {
    return value;
}

int String::getSize() {
    return sizeof *this;
}
