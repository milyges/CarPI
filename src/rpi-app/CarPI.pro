#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T20:04:59
#
#-------------------------------------------------

QT += core gui phonon
CONFIG += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarPI
TEMPLATE = app

LIBS += -lwiringPi

SOURCES += main.cpp\
        wndmain.cpp \
    displayemulator.cpp \
    navit.cpp \
    mainboard.cpp \
    wndmenu.cpp \
    changeremulator.cpp \
    mp3player.cpp \
    wndvolume.cpp \
    bluetooth.cpp \
    wndcall.cpp

HEADERS  += wndmain.h \
    displayemulator.h \
    navit.h \
    mainboard.h \
    wndmenu.h \
    changeremulator.h \
    mp3player.h \
    wndvolume.h \
    bluetooth.h \
    wndcall.h

FORMS    += wndmain.ui \
    wndmenu.ui \
    wndvolume.ui \
    wndcall.ui

RESOURCES += \
    resources.qrc
