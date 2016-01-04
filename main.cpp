#include <iostream>

#include "imemorymanager.h"
#include "pointer.h"
#include "object.h"
#include "string.h"

#include "utility.h"
#include "hashtable.h"

void run() {
    Pointer<Object> obj = new Object;

    srand(time(0));

    for (int i = 0; i < 100; i++) {
        std::cout << "> ";

        if (rand() % 5 == 0)
            new String("garbage");
        else {
            uint id = String::stringToId(Utility::toString(i));
            obj->setAttribute(id, String::idToString(id));
        }

        if (rand() % 2 == 0) {
            uint id = String::stringToId(Utility::toString(rand() % (i + 1)));

            if (obj->hasAttribute(id))
                obj->removeAttribute(id);
        }

        if (rand() % 100 == 0) {
            std::cout << "\n//random garbage collection";
            IMemoryManager::instance()->collectGarbage();
        }
    }
}

int main() {
    FILE *file = freopen("out.txt", "w", stdout);

    IMemoryManager::initialize();
    String::initialize();

    run();

    String::finalize();
    IMemoryManager::finalize();

    fclose(file);
    system("start out.txt");

    return 0;
}
