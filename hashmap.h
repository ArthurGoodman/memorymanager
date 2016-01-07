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

    class EntryReference : public ManagedObject {
        Entry *entry;

    public:
        EntryReference();

        Entry *&getEntry();

        void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
        int getSize() const;
    };

    static const int HalfInitialCapacity = 4;
    static const int LoadFactorPercent = 1000;

    EntryReference *buffer;
    int numEntries, capacity, resizeThreshold;

public:
    class iterator {
        friend class HashMap;

        EntryReference *buffer;
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
        iterator(EntryReference *buffer, int capacity, int i);
    };

    HashMap();

    iterator begin();
    iterator end();

    V get(const K &key) const;
    void put(const K &key, const V &value);
    bool remove(const K &key);
    bool contains(const K &key) const;

    int size() const;

    void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    int getSize() const;

private:
    static ulong hashKey(const K &key);

    void allocate();
    void insert(const K &key, const V &value);
    Entry *lookup(const K &key) const;
    Entry *createEntry(const K &key, const V &value) const;
};
