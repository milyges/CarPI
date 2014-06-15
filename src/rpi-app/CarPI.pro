#-------------------------------------------------
#
# Project created by QtCreator 2014-03-02T21:18:23
#
#-------------------------------------------------

QT       += core gui phonon
CONFIG += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarPI
TEMPLATE = app
LIBS += -lwiringPi


SOURCES += main.cpp\
        wndmain.cpp \
    carpi.cpp \
    mainboard.cpp \
    displayemulator.cpp \
    wndvolume.cpp \
    wndmenu.cpp \
    bluetooth.cpp \
    wndcall.cpp \
    changeremulator.cpp \
    mp3player.cpp \
    navit.cpp \
    carpidial.cpp \
    elm327.cpp

HEADERS  += wndmain.h \
    carpi.h \
    mainboard.h \
    displayemulator.h \
    wndvolume.h \
    wndmenu.h \
    bluetooth.h \
    wndcall.h \
    changeremulator.h \
    mp3player.h \
    navit.h \
    carpidial.h \
    elm327.h

FORMS    += wndmain.ui \
    wndvolume.ui \
    wndmenu.ui \
    wndcall.ui

RESOURCES += \
    resources.qrc
