TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

INCLUDEPATH += source \
    source/collections \
    source/common \
    source/memory \
    source/runtime

HEADERS += \
    source/collections/array.h \
    source/collections/hashmap.h \
    source/collections/list.h \
    source/collections/map.h \
    source/collections/sherwoodmap.h \
    source/common/common.h \
    source/common/utility.h \
    source/memory/managedobject.h \
    source/memory/markcompactmemorymanager.h \
    source/memory/memorymanager.h \
    source/memory/nomemorymanager.h \
    source/memory/pointer.h \
    source/memory/semispacememorymanager.h \
    source/runtime/object.h \
    source/runtime/staticobject.h \
    source/runtime/class.h \
    source/runtime/runtime.h \
    source/runtime/function.h \
    source/runtime/true.h \
    source/runtime/false.h \
    source/runtime/null.h \
    source/runtime/nativefunction.h \
    source/common/bytearray.h \
    source/memory/frame.h \
    source/runtime/mystring.h

SOURCES += \
    source/main.cpp \
    source/collections/array.cpp \
    source/collections/hashmap.cpp \
    source/collections/list.cpp \
    source/collections/map.cpp \
    source/collections/sherwoodmap.cpp \
    source/common/common.cpp \
    source/common/utility.cpp \
    source/memory/managedobject.cpp \
    source/memory/markcompactmemorymanager.cpp \
    source/memory/memorymanager.cpp \
    source/memory/nomemorymanager.cpp \
    source/memory/pointer.cpp \
    source/memory/semispacememorymanager.cpp \
    source/runtime/object.cpp \
    source/runtime/staticobject.cpp \
    source/runtime/class.cpp \
    source/runtime/runtime.cpp \
    source/runtime/function.cpp \
    source/runtime/true.cpp \
    source/runtime/false.cpp \
    source/runtime/null.cpp \
    source/runtime/nativefunction.cpp \
    source/common/bytearray.cpp \
    source/memory/frame.cpp \
    source/runtime/mystring.cpp
