#include <iostream>

#include "pointer.h"
#include "object.h"
#include "string.h"

#include "utility.h"

int main() {
    Pointer<Object> obj = new Object;

    //    obj->setAttribute("1", new String("one"));
    //    new String("garbage");
    //    new String("garbage");
    //    obj->setAttribute("2", new String("two"));
    //    obj->setAttribute("3", new String("three"));
    //    obj->setAttribute("4", new String("four"));
    //    obj->setAttribute("5", new String("five"));
    //    obj->setAttribute("6", new String("six"));
    //    obj->setAttribute("7", new String("seven"));
    //    obj->setAttribute("8", new String("eight"));
    //    obj->setAttribute("9", new String("nine"));
    //    obj->setAttribute("10", new String("ten"));

    //    std::cout << obj->getAttribute("2")->toString() << std::endl;

    srand(time(0));

    for (int i = 0; i < 1000; i++) {
        if (rand() % 2 == 0)
            new String(Utility::toString(i));
        else
            obj->setAttribute(Utility::toString(i), new String(Utility::toString(i)));
    }

    return 0;
}
