QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    colorpicker.cpp \
    editpatterndialog.cpp \
    ledcontroller.cpp \
    ledoutput.cpp \
    ledpattern.cpp \
    ledrgb.cpp \
    main.cpp \
    mainwindow.cpp \
    outputpaneldisplaymanager.cpp \
    patterndisplay.cpp \
    version.cpp

HEADERS += \
    aboutdialog.h \
    colorpicker.h \
    editpatterndialog.h \
    ledcontroller.h \
    ledoutput.h \
    ledpattern.h \
    ledrgb.h \
    mainwindow.h \
    outputpaneldisplaymanager.h \
    patterndisplay.h \
    version.h

FORMS += \
    aboutdialog.ui \
    editpatterndialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
