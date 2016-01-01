#pragma once

template <class T>
class Pointer {
    friend class MemoryManager;

    T *pointer;
    Pointer<T> *prev, *next;

public:
    Pointer(T *p = 0);
    Pointer(const Pointer<T> &p);
    ~Pointer();

    Pointer<T> &operator=(T *p);
    Pointer<T> &operator=(const Pointer<T> &p);

    T *operator*() const;
    T *operator->() const;
    operator T *() const;
};
