#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T10:03:26
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = logtester
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += . ../..

DESTDIR = ../../bin

LIBS += -L../../bin -ltntcore

CONFIG += c++11

HEADERS += \
    LogSimulator.h

