#include "sherwoodmap.h"

template class SherwoodMap<uint, Object *>;
template class SherwoodMap<Object *, uint>;

template <class K, class V>
V SherwoodMap<K, V>::get(const K &/*key*/) const {
    return (V)0;
}

template <class K, class V>
void SherwoodMap<K, V>::put(const K &/*key*/, const V &/*value*/) {
}

template <class K, class V>
bool SherwoodMap<K, V>::remove(const K &/*key*/) {
    return false;
}

template <class K, class V>
bool SherwoodMap<K, V>::contains(const K &/*key*/) const {
    return false;
}
