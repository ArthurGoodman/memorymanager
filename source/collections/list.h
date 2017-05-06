#pragma once

#include "managedobject.h"

template <class T>
class List : public ManagedObject {
    class ListNode : public ManagedObject {
        T value;
        ListNode *next;
    };

    int size;
    ListNode *first, *last;

public:
    List();
};
