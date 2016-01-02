#include <iostream>

#include "pointer.h"
#include "object.h"
#include "string.h"

#include "utility.h"

int main() {
    Pointer<Object> obj = new Object;

    srand(time(0));

    for (int i = 0; i < 1000; i++) {
        if (rand() % 2 == 0)
            new String(Utility::toString(i));
        else
            obj->setAttribute(Utility::toString(i), new String(Utility::toString(i)));
    }

    return 0;
}
