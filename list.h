#pragma once

#include "object.h"

class List : public Object {
    class ListNode : public Object {
        Object *value;
        ListNode *next;
    };

    int size;
    ListNode *first, *last;

public:
    List();
};
