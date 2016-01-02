#include <iostream>

#include "pointer.h"
#include "object.h"
#include "string.h"

#include "utility.h"

int main() {
    Pointer<Object> obj = new Object;

    srand(time(0));

    for (int i = 0; i < 1000; i++) {
        if (rand() % 5 == 0)
            new String(Utility::toString(i));
        else
            obj->setAttribute(Utility::toString(i), new String(Utility::toString(i)));

        if (rand() % 2 == 0) {
            std::string str = Utility::toString(rand() % (i + 1));

            if (obj->hasAttribute(str))
                obj->removeAttribute(str);
        }
    }

    return 0;
}
