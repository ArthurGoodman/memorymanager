#pragma once

#include <functional>

#include "common.h"

class ManagedObject {
    uint flags;
    ManagedObject *forwardAddress;

public:
    enum {
        FlagMark = 1 << 0
    };

    static void *operator new(uint size) noexcept;
    static void *operator new(uint size, void *p) noexcept;
    static void operator delete(void *p) noexcept;

    ManagedObject();
    virtual ~ManagedObject();

    bool hasFlag(int flag) const;
    void setFlag(int flag);
    void removeFlag(int flag);

    ManagedObject *getForwardAddress() const;
    void setForwardAddress(ManagedObject *forwardAddress);

    virtual void mapOnReferences(const std::function<void(ManagedObject *&)> &f);
    virtual int getSize() const = 0;
};

inline bool ManagedObject::hasFlag(int flag) const {
    return flags & flag;
}

inline void ManagedObject::setFlag(int flag) {
    flags |= flag;
}

inline void ManagedObject::removeFlag(int flag) {
    flags &= ~flag;
}

inline ManagedObject *ManagedObject::getForwardAddress() const {
    return forwardAddress;
}

inline void ManagedObject::setForwardAddress(ManagedObject *forwardAddress) {
    this->forwardAddress = forwardAddress;
}
