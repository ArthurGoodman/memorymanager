#pragma once

#include "map.h"

template <class>
class Array;

template <class K, class V>
class HashMap : public Map<K, V> {
    class Entry : public ManagedObject {
        K key;
        V value;

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

    friend class Array<Entry *>;

    static const int HalfInitialCapacity = 4;
    static const int LoadFactorPercent = 1000;

    Array<Entry *> *buffer;
    int numEntries, capacity, resizeThreshold, mask;

public:
    class iterator {
        friend class HashMap;

        Array<Entry *> *buffer;
        Entry *entry;
        int capacity, i;

    public:
        iterator &operator++();
        iterator &operator*();

        bool operator!=(const iterator &other) const;

        K &key();
        V &value();

    private:
        iterator();
        iterator(Array<Entry *> *buffer, int capacity, int i);
    };

    HashMap();

    iterator begin();
    iterator end();

    V &get(const K &key) const;
    V &put(const K &key, const V &value);
    bool remove(const K &key);
    bool contains(const K &key) const;

    int size() const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;

private:
    static ulong hashKey(const K &key);

    void allocate();
    V &insert(const K &key, const V &value);
    Entry *lookup(const K &key) const;
    Entry *createEntry(const K &key, const V &value) const;
};
