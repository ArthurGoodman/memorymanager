#pragma once

#include "object.h"

class HashTable : public Object {
    class HashNode : public Object {
        std::string key;
        Object *value;
        HashNode *next;

    public:
        HashNode(std::string key, const Pointer<Object> value);

        std::string &getKey();
        Object *&getValue();

        void setValue(Object *value);

        HashNode *getNext() const;
        void setNext(HashNode *next);

        void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
        int getSize();
    };

    static const int HashTableSize = 10;

    std::hash<std::string> hashFunction;
    HashNode *table[HashTableSize];

public:
    class iterator {
        friend class HashTable;

        HashNode **table;

        int i;
        HashNode *node;

    public:
        iterator &operator++();
        iterator &operator*();

        bool operator!=(const iterator &other) const;

        std::string &key();
        Object *&value();

    private:
        iterator(HashNode **table);
        iterator(HashNode **table, int i);
    };

    HashTable();

    iterator begin();
    iterator end();

    Object *get(const std::string &key) const;
    void put(const std::string &key, const Pointer<Object> &value);
    bool remove(const std::string &key);
    bool contains(const std::string &key);

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize();
};
