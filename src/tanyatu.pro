TEMPLATE = subdirs

SUBDIRS += core
SUBDIRS += uicommon
SUBDIRS += desktop
desktop.depends = core uicommon

QMAKE_CXXFLAGS += -frtt


*-g++* {
    QMAKE_CXXFLAGS += -std=c++11
}
