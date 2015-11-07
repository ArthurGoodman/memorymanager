#pragma once
#include "object.h"

class List;

class HashTable : public Object {
    static const int BucketsCount = 100;

    List *buckets[BucketsCount];
    int size;

public:
    HashTable();

    int getSize();
};
