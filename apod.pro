#-------------------------------------------------
#
# Project created by QtCreator 2016-05-08T11:26:27
#
#-------------------------------------------------

QT += core gui
QT += network
DEFINES  += QT_NO_SSL

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = apod
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    spaceimages.cpp \
    spaceimage.cpp \
    httpdownload.cpp

HEADERS  += mainwindow.h \
    spaceimages.h \
    spaceimage.h \
    httpdownload.h

FORMS    += mainwindow.ui

DISTFILES += \
    api.txt
