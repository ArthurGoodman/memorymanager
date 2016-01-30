#pragma once

#include "common.h"
#include "frame.h"

class ManagedObject;

template <class>
class Pointer;

class MemoryManager {
protected:
    static Pointer<ManagedObject> *&pointers();
    static Frame *&frames();

public:
    static MemoryManager *instance();

    static void registerPointer(Pointer<ManagedObject> *pointer);
    static void removePointer(Pointer<ManagedObject> *pointer);

    static void registerFrame(Frame *frame);
    static void removeFrame(Frame *frame);

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

#define GC_OPEN_FRAME \
    Frame __gc_frame__([&](std::function<void(ManagedObject *&)> __gc_f__) {

#define GC_OPEN_STATIC_FRAME \
    static GC_OPEN_FRAME

#define GC_CLOSE_FRAME \
    });

#define GC_FRAME(args) \
    GC_OPEN_FRAME args GC_CLOSE_FRAME

#define GC_STATIC_FRAME(args) \
    static GC_FRAME(args)

#define POINTER(pointer) \
    if (pointer != 0)    \
        __gc_f__((ManagedObject *&)pointer);

#define LIST(list)                    \
    for (auto &__gc_pointer__ : list) \
        if (__gc_pointer__ != 0)      \
            __gc_f__((ManagedObject *&)__gc_pointer__);
