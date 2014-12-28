
QT           += core widgets
CONFIG       += qt shared
DEFINES      += TANYATU_UI_COMMON
LIBS         += -L../bin -ltntcore
INCLUDEPATH += $$PWD/../../lib/taglib/include
LIBS        += -L$$PWD/../../lib/taglib/lib -ltag
linux-g++ {
    LIBS += -L/usr/lib/x86_64-linux-gnu/mesa -lGL
            #-lphonon4qt5
}


TEMPLATE    = lib

TARGET        = tntuicommon
DESTDIR       = ../bin
INCLUDEPATH  += ./.. \


HEADERS += \
    TanyatuCommonGlobal.h \

include(itemmodels/itemmodels.pri)
include(uiutils/uiutils.pri)

*-g++* {
    QMAKE_CXXFLAGS += -std=c++11
}


