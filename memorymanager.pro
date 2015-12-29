TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

SOURCES += main.cpp \
    vector.cpp \
    memorymanager.cpp \
    common.cpp \
    object.cpp \
    list.cpp \
    hashtable.cpp \
    pointer.cpp

HEADERS += \
    vector.h \
    memorymanager.h \
    common.h \
    object.h \
    list.h \
    hashtable.h \
    pointer.h
