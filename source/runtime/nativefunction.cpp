#include "nativefunction.h"

NativeFunction::NativeFunction(const std::string &name, const std::function<Object *(Object *, const std::list<Object *> &)> &body)
    : Function(name), body(body) {
}

Object *NativeFunction::invoke(Object *self, const std::list<Object *> &args) {
    return body(self, args);
}
