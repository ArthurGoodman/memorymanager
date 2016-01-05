#include <iostream>

#include "memorymanager.h"
#include "pointer.h"
#include "object.h"
#include "string.h"

#include "utility.h"

#include "sherwoodmap.h"

void run() {
    //    Pointer<Object> obj = new Object;

    //    srand(time(0));

    //    for (int i = 0; i < 100; i++) {
    //        std::cout << "> ";

    //        if (rand() % 5 == 0)
    //            new String("garbage");
    //        else {
    //            uint id = String::stringToId(Utility::toString(i));
    //            obj->setAttribute(id, String::idToString(id));
    //        }

    //        if (rand() % 2 == 0) {
    //            uint id = String::stringToId(Utility::toString(rand() % (i + 1)));

    //            if (obj->hasAttribute(id))
    //                obj->removeAttribute(id);
    //        }

    //        if (rand() % 100 == 0) {
    //            std::cout << "\n//random garbage collection";
    //            MemoryManager::instance()->collectGarbage();
    //        }
    //    }

    Pointer<Map<uint, Object *>> map = new SherwoodMap<uint, Object *>;

    map->put(1, new String("one"));
    map->put(2, new String("two"));
    map->put(3, new String("three"));

    std::cout << map->get(2) << "\n";
}

int main() {
    //    FILE *file = freopen("out.txt", "w", stdout);

    MemoryManager::initialize();
    String::initialize();

    run();

    String::finalize();
    MemoryManager::finalize();

    //    fclose(file);
    //    system("start out.txt");

    return 0;
}
