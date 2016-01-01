#pragma once

#include "object.h"

template <class T>
class List : public Object {
    class ListNode : public Object {
        T value;
        ListNode *next;
    };

    int size;
    ListNode *first, *last;

public:
    List();
};
