#-------------------------------------------------
#
# Project created by QtCreator 2016-05-08T11:26:27
#
#-------------------------------------------------

QT += core gui
QT += network
QT += sql
DEFINES  += QT_NO_SSL

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = apod
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    downloadfile.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    downloadfile.h

FORMS    += mainwindow.ui

DISTFILES += \
    api.txt
