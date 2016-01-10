#include "function.h"

Function::Function(const std::string &name)
    : name(name) {
}

Object *Function::invoke(Object *self, const std::list<Object *> &args) {
    return 0;
}

std::string Function::toString() {
    return name.empty() ? "<anonymous function>" : "<function " + name + ">";
}
