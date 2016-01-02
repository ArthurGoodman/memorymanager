#include <iostream>

#include "pointer.h"
#include "object.h"
#include "string.h"

int main() {
    Pointer<Object> obj = new Object;

    new String("garbage");
    obj->setAttribute("1", new String("one"));
    obj->setAttribute("2", new String("two"));
    obj->setAttribute("3", new String("three"));
    obj->setAttribute("4", new String("four"));
    obj->setAttribute("5", new String("five"));
    obj->setAttribute("6", new String("six"));
    obj->setAttribute("7", new String("seven"));
    obj->setAttribute("8", new String("eight"));
    obj->setAttribute("9", new String("nine"));
    obj->setAttribute("10", new String("ten"));

    std::cout << obj->getAttribute("2")->toString() << std::endl;

    return 0;
}
