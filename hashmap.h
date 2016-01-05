#pragma once

#include "map.h"

template <class K, class V>
class HashMap : public Map<K, V> {
    class Entry : public Map<K, V>::Entry {
        Entry *next;

    public:
        Entry(const K &key, const V &value);

        K &getKey();
        V &getValue();

        void setKey(const K &key);
        void setValue(const V &value);

        Entry *getNext() const;
        void setNext(Entry *next);

        bool equals(const K &key) const;

        void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
        int getSize() const;
    };

    static const int TableSize = 10;

    Entry *table[TableSize];

public:
    class iterator {
        friend class HashMap;

        Entry **table, *entry;
        int i;

    public:
        iterator &operator++();
        iterator &operator*();

        bool operator!=(const iterator &other) const;

        K &key();
        V &value();

    private:
        iterator();
        iterator(Entry **table, int i);
    };

    HashMap();

    iterator begin();
    iterator end();

    V get(const K &key) const;
    void put(const K &key, const V &value);
    bool remove(const K &key);
    bool contains(const K &key) const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;

private:
    static ulong hashKey(const K &key);

    Entry *createEntry(const K &key, const V &value) const;
};
