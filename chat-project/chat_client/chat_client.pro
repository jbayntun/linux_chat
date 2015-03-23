#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T14:53:28
#
#-------------------------------------------------

QT       += core gui
QT      += opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    network.cpp \
    helpers.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    network.h \
    helpers.h \
    mythread.h

FORMS    += mainwindow.ui

