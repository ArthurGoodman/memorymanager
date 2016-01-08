#include <iostream>

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

            if (obj->hasAttribute(id)) {
                if (rand() % 2 == 0)
                    obj->removeAttribute(id);
                else {
                    String *str = String::idToString(id);
                    obj->setAttribute(id, str);
                }
            }
        }

        if (rand() % n == 0) {
            std::cout << "\n//random garbage collection";
            MemoryManager::instance()->collectGarbage();
        }
    }
}

#include "sherwoodmap.h"

int main() {
    //    FILE *file = freopen("out.txt", "w", stdout);

    MemoryManager::initialize();
    String::initialize();

    double t = clock();

    //    run();

    {
        Pointer<Map<Object *, uint>> map = new SherwoodMap<Object *, uint>;
        String *str;

        for (int i = 0; i < 34; i++) {
            str = new String(Utility::toString(i));
            map->put(str, i);
        }

        str = new String("4");
        std::cout << "\n" << (map->contains(str) ? "true" : "false") << "\n";
    }

    std::cout << "\nElapsed: " << (clock() - t) / CLOCKS_PER_SEC << " sec.\n";

    String::finalize();
    MemoryManager::finalize();

    //    fclose(file);
    //    system("start out.txt");

    return 0;
}
