QT += core serialport testlib
QT += gui widgets
QT += svg printsupport network

[Rules]
*.debug=true

message($$PWD)

win32 {
    COPY_FROM_PATH=$$shell_path($$PWD/KX_pult/protocols.conf)
    COPY_TO_PATH=$$shell_path($$OUT_PWD)
}
else {
    COPY_FROM_PATH=$$PWD/KX_pult/protocols.conf
    COPY_TO_PATH=$$OUT_PWD
}

copydata.commands = $(COPY_DIR) $$COPY_FROM_PATH $$COPY_TO_PATH
first.depends = $(first) copydata

export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata

TARGET = Iceberg
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    mainwindow.cpp \
    joystick.cpp \
    settings.cpp \
    logging_categories.cpp \
    KX_pult/configdata.cpp \
    KX_pult/kx_protocol.cpp \
    KX_pult/qkx_coeffs.cpp \
    KX_pult/qpiconfig.cpp \
    KX_pult/rov_model.cpp \

HEADERS += \
    server.h \
    messages.h \
    mainwindow.h \
    joystick.h \
    settings.h \
    logging_categories.h \
    KX_pult/configdata.h \
    KX_pult/kx_protocol.h \
    KX_pult/qkx_coeffs.h \
    KX_pult/qpiconfig.h \
    KX_pult/rov_model.h \

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
    pictures.qrc
