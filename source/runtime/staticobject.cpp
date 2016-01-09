#include "staticobject.h"

#include "memorymanager.h"

StaticObject *StaticObject::create(int refCount) {
    ManagedObject *object = MemoryManager::instance()->allocate(sizeof(StaticObject) + refCount * sizeof(ManagedObject *));
    new (object) StaticObject(refCount);
    new ((byte *)object + sizeof(StaticObject)) ManagedObject *[refCount] {0 };
    return (StaticObject *)object;
}

ManagedObject *&StaticObject::field(int index) {
    return *(ManagedObject **)((byte *)this + sizeof(*this) + index * sizeof(ManagedObject *));
}

void StaticObject::mapOnReferences(const std::function<void(ManagedObject *&)> &f) {
    for (int i = 0; i < refCount; i++)
        if (field(i))
            f(field(i));
}

int StaticObject::getSize() const {
    return sizeof(*this) + refCount * sizeof(ManagedObject *);
}

StaticObject::StaticObject(int refCount)
    : refCount(refCount) {
    for (int i = 0; i < refCount; i++)
        field(i) = 0;
}
