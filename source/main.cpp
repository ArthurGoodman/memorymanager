#include <iostream>

#include "pointer.h"
#include "object.h"
#include "string.h"
#include "utility.h"
#include "runtime.h"
#include "class.h"

void run() {
    Pointer<Object> obj = new Object;

    srand(time(0));

    const int n = 100;

    for (int i = 0; i < n; i++) {
        std::cout << "> ";

        if (rand() % 5 == 0)
            new String("garbage");
        else {
            uint id = String::id(Utility::toString(i));
            obj->setAttribute(id, String::string(id));
        }

        if (rand() % 2 == 0) {
            uint id = String::id(Utility::toString(rand() % (i + 1)));

            if (obj->hasAttribute(id)) {
                if (rand() % 2 == 0)
                    obj->removeAttribute(id);
                else {
                    String *str = String::string(id);
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

int main() {
    //    FILE *file = freopen("out.txt", "w", stdout);

    MemoryManager::initialize();
    String::initialize();
    Runtime::initialize();

    double t = clock();

    //    run();

    {
        std::cout << "\n";

        std::cout << Runtime::getObjectClass()->toString() << "\n";
        std::cout << Runtime::getMainObject()->call("class")->toString() << "\n";
        std::cout << Runtime::getObjectClass()->call("class")->toString() << "\n";
    }

    std::cout << "\nElapsed: " << (clock() - t) / CLOCKS_PER_SEC << " sec\n";

    Runtime::finalize();
    String::finalize();
    MemoryManager::finalize();

    //    fclose(file);
    //    system("start out.txt");

    return 0;
}
