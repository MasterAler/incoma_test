QT -= gui
QT += core network

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
        RequestHandler.cpp \
        RequestServer.cpp \
        ResponderThread.cpp \
        SimpleServerContainer.cpp \
        main.cpp

include(../common/common.pri)
include(../thirdparty/logger.pri)
LIBS *= -L$$DESTDIR -lCuteLogger

HEADERS += \
    RequestHandler.h \
    RequestServer.h \
    ResponderThread.h \
    SimpleServerContainer.h
