TEMPLATE = app
INCLUDEPATH = . ..
QT  += widgets core sql multimedia
DESTDIR = ../bin
LIBS += -L../bin -ltntuicommon -ltntcore



TARGET = GreenChilli

include( app/app.pri )
include( desktoputils/desktoputils.pri )
include( historyview/historyview.pri )
include( libview/libview.pri )
include( player/player.pri )
include( playlistview/playlistview.pri )
include( playqueueview/playqueueview.pri )
include( preferenceview/preferenceview.pri )
include( trackview/trackview.pri )
include( viewcommon/viewcommon.pri )
include( widgets/widgets.pri )
include( thirdparty/thirdparty.pri )

linux-g++ {
    LIBS += -L/usr/lib/x86_64-linux-gnu/mesa -lGL
    LIBS += -L/usr/lib/x86_64-linux-gnu -lphonon4qt5
}

RESOURCES += \
    images.qrc \
    
FORMS += \
    AudioPlayer.ui \
    
SOURCES += \
    main.cpp \
    
HEADERS += \
    

*-g++* {
    QMAKE_CXXFLAGS += -std=c++11
}
