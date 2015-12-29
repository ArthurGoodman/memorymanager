#include "object.h"

//#include "memorymanager.h"
//#include "hashtable.h"

Object::Object()
/*: attributes(0)*/ {
}

void Object::mark() {
    setFlag(FlagMark);

    //    if (attributes)
    //        attributes->mark();
}

int Object::getSize() {
    return sizeof *this;
}
