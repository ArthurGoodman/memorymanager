#pragma once

class Object;
class MemoryManager;

class Pointer {
    friend class MemoryManager;

    const unsigned magic = 0xdeadbeef;

    Object *object;
    Pointer *prev, *next;

public:
    Pointer(Object *object = 0);
    Pointer(const Pointer &object);
    ~Pointer();

    Pointer &operator=(Object *object);
    Pointer &operator=(const Pointer &p);

    Object *operator*();
    Object *operator->();

    void shift(int delta);
};
