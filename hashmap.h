#pragma once

#include "object.h"

template <class K, class V>
class HashMap : public Object {
protected:
    class Entry : public Object {
    protected:
        K key;
        V value;

    public:
        Entry(const K &key, const V &value);
    };

public:
//    class iterator {
//    public:
//        virtual iterator &operator++();
//        virtual iterator &operator*();

//        virtual bool operator!=(const iterator &other) const;

//        virtual K &key();
//        virtual V &value();
//    };

//    virtual iterator begin() = 0;
//    virtual iterator end() = 0;

    virtual V get(const K &key) const = 0;
    virtual void put(const K &key, const V &value) = 0;
    virtual bool remove(const K &key) = 0;
    virtual bool contains(const K &key) const = 0;
};

template <class K, class V>
HashMap<K, V>::Entry::Entry(const K &key, const V &value)
    : key(key), value(value) {
}

//template <class K, class V>
//typename HashMap<K, V>::iterator &HashMap<K, V>::iterator::operator++() {
//    return *this;
//}

//template <class K, class V>
//typename HashMap<K, V>::iterator &HashMap<K, V>::iterator::operator*() {
//    return *this;
//}

//template <class K, class V>
//bool HashMap<K, V>::iterator::operator!=(const iterator &) const {
//    return false;
//}

//template <class K, class V>
//K &HashMap<K, V>::iterator::key() {
//    throw std::exception();
//}

//template <class K, class V>
//V &HashMap<K, V>::iterator::value() {
//    throw std::exception();
//}
