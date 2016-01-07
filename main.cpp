#include <iostream>

#include "pointer.h"
#include "object.h"
#include "string.h"
#include "utility.h"

void run() {
    Pointer<Object> obj = new Object;

    srand(time(0));

    const int n = 100;

    int num = 0;

    for (int i = 0; i < n; i++) {
        std::cout << "> ";

        if (rand() % 5 == 0)
            new String("garbage");
        else {
            uint id = String::stringToId(Utility::toString(i));
            obj->setAttribute(id, String::idToString(id));
            num++;
        }

        if (rand() % 2 == 0) {
            uint id = String::stringToId(Utility::toString(rand() % (i + 1)));

            if (obj->hasAttribute(id)) {
                obj->removeAttribute(id);
                num--;
            }
        }

        if (rand() % n == 0) {
            std::cout << "\n//random garbage collection";
            MemoryManager::instance()->collectGarbage();
        }
    }
}

#include <sherwoodmap.h>

int main() {
    FILE *file = freopen("out.txt", "w", stdout);

    MemoryManager::initialize();
    String::initialize();

    double t = clock();

    //    run();

    {
        Pointer<Map<uint, Object *>> map = new SherwoodMap<uint, Object *>;

        map->put(1, new String("1"));
        map->put(2, new String("2"));
        map->put(3, new String("3"));
        map->put(4, new String("4"));

        map->put(3, new String("lel"));

        std::cout << map->size() << "\n"; // should be 4
    }

    std::cout << "\nElapsed: " << (clock() - t) / CLOCKS_PER_SEC << " sec.\n";

    String::finalize();
    MemoryManager::finalize();

    fclose(file);
    system("start out.txt");

    return 0;
}
