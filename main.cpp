#include <iostream>

#include "memorymanager.h"
#include "pointer.h"
#include "object.h"
#include "string.h"

#include "utility.h"

void run() {
    Pointer<Object> obj = new Object;

    srand(time(0));

    const int n = 100;

    for (int i = 0; i < n; i++) {
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

        if (rand() % n == 0) {
            std::cout << "\n//random garbage collection";
            MemoryManager::instance()->collectGarbage();
        }
    }
}

int main() {
    FILE *file = freopen("out.txt", "w", stdout);

    MemoryManager::initialize();
    String::initialize();

    run();

    String::finalize();
    MemoryManager::finalize();

    fclose(file);
    system("start out.txt");

    return 0;
}
