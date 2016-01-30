#pragma once

#include <functional>

class ManagedObject;

class Frame {
    std::function<void(const std::function<void(ManagedObject *&)> &)> map;

public:
    explicit Frame(const std::function<void(const std::function<void(ManagedObject *&)> &)> &map);
    ~Frame();

    void mapOnLocals(const std::function<void(ManagedObject *&)> &map);
};
