
QT       += core sql multimedia

DESTDIR = ../bin
TARGET = tntcore

TEMPLATE = lib
CONFIG += qt shared

CCFLAG += -fPIC
#CONFIG += debug


include(data/data.pri)
include(interfaces/interfaces.pri)
include(impl/impl.pri)
include(datasource/datasource.pri)
include(coreutils/coreutils.pri)
include(logger/logger.pri)



HEADERS += \
    T.h \
    TanyatuCoreGlobal.h \

SOURCES += \
    T.cpp \
    


DEFINES += TANYATU_CORE

linux-g++-64  {
    LIBS += -L/usr/lib/x86_64-linux-gnu -lphonon4qt5
    LIBS += -L/usr/lib/x86_64-linux-gnu/mesa -lGL
}

*-g++* {
    QMAKE_CXXFLAGS += -std=c++11
}

