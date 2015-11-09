#pragma once

#include "object.h"

template <typename T>
class List : public Object {
    class ListNode : public Object {
        T value;
        ListNode *next;
    };

    int size;
    ListNode *first, *last;

public:
    List()
        : size(0), first(0), last(0) {
    }

    int getSize() const {
        return size;
    }
};
