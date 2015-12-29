#pragma once

class Object;
class MemoryManager;

class Pointer {
    friend class MemoryManager;

    Object *object;
    Pointer *prev, *next;

public:
    Pointer(Object *object = 0);
    ~Pointer();

    void shift(int delta);

    Object *operator*();
    Object *operator->();

    operator Object *();
};
