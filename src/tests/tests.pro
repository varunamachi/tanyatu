TEMPLATE = subdirs

SUBDIRS += \
    logtester

*-g++* {
    QMAKE_CXXFLAGS += -std=c++11
}
