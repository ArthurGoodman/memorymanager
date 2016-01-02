#pragma once

#include <list>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long long ulong;

template <class A, class B>
std::list<B> &operator<<(std::list<B> &l, A &e) {
    l.push_back((B)e);
    return l;
}
