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
    imemorymanager.cpp \
    bytearray.cpp \
    localmemorymanager.cpp \
    nomemorymanager.cpp

HEADERS += \
    common.h \
    object.h \
    list.h \
    hashtable.h \
    pointer.h \
    managedobject.h \
    string.h \
    utility.h \
    imemorymanager.h \
    bytearray.h \
    localmemorymanager.h \
    nomemorymanager.h
