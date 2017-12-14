QT += core serialport testlib
QT += gui widgets
QT += svg
QT += printsupport

TARGET = IcebergNOGUI
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    mainwindow.cpp \
    joystick.cpp \
    settings.cpp \
    qfi_PFD.cpp \
    newwindow.cpp \
    logging_categories.cpp \
    qcustomplot.cpp

HEADERS += \
    server.h \
    messages.h \
    mainwindow.h \
    joystick.h \
    settings.h \
    qfi_PFD.h \
    newwindow.h \
    logging_categories.h \
    qcustomplot.h

DEFINES += SFML_STATIC

LIBS += -L$$PWD/SFML-win32/lib

INCLUDEPATH += $$PWD/SFML-win32/include

DEPENDPATH += $$PWD/SFML-win32/include

CONFIG(release, debug|release): LIBS += -lsfml-window
CONFIG(debug, debug|release): LIBS += -lsfml-window-d

FORMS += \
    mainwindow.ui \
    bluetooth.ui

RESOURCES += \
    qfi.qrc \
    pictures.qrc
