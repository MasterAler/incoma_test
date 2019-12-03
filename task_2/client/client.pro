QT -= gui
QT += core network

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
        ClientRandomRequestGenerator.cpp \
        Request.cpp \
        main.cpp

include(../common/common.pri)
include(../thirdparty/logger.pri)
LIBS *= -L$$DESTDIR -lCuteLogger

HEADERS += \
    ClientRandomRequestGenerator.h \
    Request.h
