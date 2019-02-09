QT += core serialport testlib
QT += gui widgets
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT+= widgets printsupport

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
    logging_categories.cpp \
    qcustomplot.cpp

HEADERS += \
    server.h \
    messages.h \
    mainwindow.h \
    joystick.h \
    settings.h \
    qfi_PFD.h \
    logging_categories.h \
    qcustomplot.h

DEFINES += SFML_STATIC

unix{
    DEFINES += OS=\\\"unix\\\"
    message("Unix build")
    INCLUDEPATH += $$PWD/SFML-linux/include
    DEPENDPATH += $$PWD/SFML-linux/include
    LIBS += -L$$PWD/SFML-linux/lib
    CONFIG(release, debug|release): LIBS += -lsfml-window
    CONFIG(debug, debug|release): LIBS += -lsfml-window
}

win32{
    DEFINES += OS=\\\"win32\\\"
    message("Windows build")
    INCLUDEPATH += $$PWD/SFML-win32/include
    DEPENDPATH += $$PWD/SFML-win32/include
    LIBS += -L$$PWD/SFML-win32/lib
    CONFIG(release, debug|release): LIBS += -lsfml-window
    CONFIG(debug, debug|release): LIBS += -lsfml-window-d
}

FORMS += \
    mainwindow.ui

RESOURCES += \
    qfi.qrc \
    pictures.qrc
