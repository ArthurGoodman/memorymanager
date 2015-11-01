TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

SOURCES += main.cpp \
    vector.cpp \
    memorymanager.cpp \
    common.cpp

HEADERS += \
    vector.h \
    memorymanager.h \
    common.h
