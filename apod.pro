#-------------------------------------------------
#
# Project created by QtCreator 2016-05-08T11:26:27
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = apod
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        downloadfile.cpp

HEADERS  += mainwindow.h \
    downloadfile.h

FORMS    += mainwindow.ui

RESOURCES += \
    apodviewer.qrc
