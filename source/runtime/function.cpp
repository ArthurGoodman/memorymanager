#include "function.h"

Function::Function(const std::string &name)
    : Object("Function"), name(name) {
}

std::string Function::toString() const {
    return name.empty() ? "<anonymous function>" : "<function " + name + ">";
}
