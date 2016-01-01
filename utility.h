#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

class Utility {
public:
    template <class T>
    static std::string toString(T t, int base = 10, int precision = 16) {
        std::ostringstream s;
        s << std::setbase(base) << std::setprecision(precision) << t;
        return s.str();
    }

    template <class T>
    static T fromString(std::string str) {
        std::istringstream s(str);
        T t;
        s >> t;
        return t;
    }
};

template <>
inline double Utility::fromString<double>(std::string str) {
    if (str == "inf")
        return INFINITY;
    if (str == "nan")
        return NAN;
    std::istringstream s(str);
    double t;
    s >> t;
    return t;
}
