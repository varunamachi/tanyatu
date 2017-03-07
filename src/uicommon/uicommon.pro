
QT           += core widgets
CONFIG       += qt shared
DEFINES      += TANYATU_UI_COMMON
DEFINES      += TAGLIB_STATIC
LIBS         += -L../bin -ltntcore

TEMPLATE    = lib

TARGET        = tntuicommon
DESTDIR       = ../bin
INCLUDEPATH  += ./.. \

*-g++* {
    QMAKE_CXXFLAGS += -std=c++11
}

TAGLIB_NAME = tag
CONFIG(debug, debug|release) {
    TAGLIB_NAME = tagd
}

win32-msvc* {
    MSVC_VER = $$(VisualStudioVersion)
    equals(MSVC_VER, 12.0){
        INCLUDEPATH += $$PWD/../../lib/taglib/include
        LIBS        += -L$$PWD/../../lib/taglib/lib/msvc12_x86 -l$${TAGLIB_NAME}
    }
}
win32-g++* {
    INCLUDEPATH += $$PWD/../../lib/taglib/include
    LIBS        += -L$$PWD/../../lib/taglib/lib/mingw_x86 -l$${TAGLIB_NAME}
}
linux-g++ {
    LIBS += -L/usr/lib/x86_64-linux-gnu/mesa -lGL
    LIBS += -L/usr/lib/x86_64-linux-gnu -lphonon4qt5
}

HEADERS += \
    TanyatuCommonGlobal.h \

include(itemmodels/itemmodels.pri)
include(uiutils/uiutils.pri)
