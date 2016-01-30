#pragma once

#include <list>

#include "common.h"
#include "frame.h"

class ManagedObject;

template <class>
class Pointer;

class MemoryManager {
protected:
    static Pointer<ManagedObject> *&pointers();
    static std::list<Frame *> &frames();

public:
    static MemoryManager *instance();

    static void registerPointer(Pointer<ManagedObject> *pointer);
    static void removePointer(Pointer<ManagedObject> *pointer);

    static void pushFrame(Frame *frame);
    static void popFrame();

    template <class T>
    T *allocateArray(uint size);

    virtual ~MemoryManager();

    virtual ManagedObject *allocate(uint size, int count = 1) = 0;
    virtual void free(ManagedObject *p) = 0;

    virtual void collectGarbage() = 0;
};

template <class T>
T *MemoryManager::allocateArray(uint size) {
    return new (instance()->allocate(size * sizeof(T), size)) T[size];
}

#define GC_FRAME(GC_ARGS)                                                         \
    Frame __gc_frame__([&](std::function<void(ManagedObject *&)> __gc_update__) { \
        GC_ARGS                                                                   \
    });

#define POINTER(pointer) \
    if (pointer != 0)   \
        __gc_update__((ManagedObject *&)pointer);

#define LIST(list)                  \
    for (auto &__gc_pointer__ : list) \
        if (__gc_pointer__ != 0)      \
            __gc_update__((ManagedObject *&)__gc_pointer__);
