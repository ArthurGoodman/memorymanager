#include "hashtable.h"

#include "list.h"

HashTable::HashTable()
    : size(0) {
    for (List *&list : buckets)
        list = new List();
}

int HashTable::getSize() {
    return size;
}
