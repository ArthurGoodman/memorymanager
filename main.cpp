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
        if (rand() % 5 == 0)
            new String("garbage");
        else
            obj->setAttribute(Utility::toString(i), String::idToString(String::stringToId(Utility::toString(i))));

        if (rand() % 2 == 0) {
            std::string str = Utility::toString(rand() % (i + 1));

            if (obj->hasAttribute(str))
                obj->removeAttribute(str);
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
