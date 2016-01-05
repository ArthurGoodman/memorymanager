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

class StaticObject : public ManagedObject {
    int refCount;

public:
    static StaticObject *create(int refCount) {
        ManagedObject *object = MemoryManager::instance()->allocate(sizeof(StaticObject) + refCount * sizeof(ManagedObject *));
        new (object) StaticObject(refCount);
        return (StaticObject *)object;
    }

    StaticObject(int refCount)
        : refCount(refCount) {
        for (int i = 0; i < refCount; i++)
            field(i) = 0;
    }

    ManagedObject *&field(int index) {
        return *(ManagedObject **)((byte *)this + sizeof(*this) + index * sizeof(ManagedObject *));
    }

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
        for (int i = 0; i < refCount; i++)
            if (field(i))
                f(field(i));
    }

    int getSize() const {
        return sizeof(*this) + refCount * sizeof(ManagedObject *);
    }
};

int main() {
    //    FILE *file = freopen("out.txt", "w", stdout);

    MemoryManager::initialize();
    //    String::initialize();

    //    run();

    {
        Pointer<ManagedObject> object = StaticObject::create(3);

        ((StaticObject *)*object)->field(0) = new String("one");
        ((StaticObject *)*object)->field(1) = new String("two");
        ((StaticObject *)*object)->field(2) = new String("three");

        for (int i = 0; i < 47; i++)
            new String("garbage");

        std::cout << ((Object *)((StaticObject *)*object)->field(1))->toString() << "\n";
        std::cout << ((Object *)((StaticObject *)*object)->field(2))->toString() << "\n";
        std::cout << ((Object *)((StaticObject *)*object)->field(0))->toString() << "\n";
    }

    //    String::finalize();
    MemoryManager::finalize();

    //    fclose(file);
    //    system("start out.txt");

    return 0;
}
