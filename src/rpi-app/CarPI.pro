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
    carpi.cpp \
    mainboard.cpp \
    displayemulator.cpp \    
    bluetooth.cpp \
    changeremulator.cpp \
    mp3player.cpp \
    navit.cpp \
    carpidial.cpp \
    elm327.cpp \
    wndmain.cpp \
    scrolledlabel.cpp \
    wndvolume.cpp \
    menumodule.cpp \
    navigationmodule.cpp \
    dashboardmodule.cpp \
    radiomodule.cpp \ 
    wndcallinfo.cpp

HEADERS  += carpi.h \
    mainboard.h \
    displayemulator.h \
    bluetooth.h \
    changeremulator.h \
    mp3player.h \
    navit.h \
    carpidial.h \
    elm327.h \
    wndmain.h \
    scrolledlabel.h \
    wndvolume.h \
    guimodule.h \
    menumodule.h \
    navigationmodule.h \
    dashboardmodule.h \
    radiomodule.h \ 
    wndcallinfo.h

FORMS    += \
    wndmain.ui \
    wndvolume.ui \
    radioModule.ui \ 
    wndcallinfo.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    themes/default.css
