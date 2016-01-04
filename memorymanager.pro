TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

SOURCES += main.cpp \
    common.cpp \
    object.cpp \
    list.cpp \
    hashtable.cpp \
    pointer.cpp \
    managedobject.cpp \
    string.cpp \
    utility.cpp \
    bytearray.cpp \
    nomemorymanager.cpp \
    memorymanager.cpp \
    markcompactmemorymanager.cpp

HEADERS += \
    common.h \
    object.h \
    list.h \
    hashtable.h \
    pointer.h \
    managedobject.h \
    string.h \
    utility.h \
    bytearray.h \
    nomemorymanager.h \
    memorymanager.h \
    markcompactmemorymanager.h
