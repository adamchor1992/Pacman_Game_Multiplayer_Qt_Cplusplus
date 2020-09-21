#-------------------------------------------------
#
# Project created by QtCreator 2018-05-03T14:25:18
#
#-------------------------------------------------

QT += core gui
QT += widgets
QT += multimedia
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pacman_Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ask_for_ip_interface.cpp \
    client_connection.cpp \
    game_window.cpp \
        main.cpp \
    ghost.cpp \
    map.cpp \
    pacman.cpp \
    foodball.cpp \
    powerball.cpp \
    sounds.cpp \
    text_screen_message.cpp

HEADERS += \
    ask_for_ip_interface.h \
    client_connection.h \
    game_window.h \
    ghost.h \
    map.h \
    pacman.h \
    foodball.h \
    powerball.h \
    sounds.h \
    text_screen_message.h

FORMS += \
    ask_for_ip_interface.ui \
    game_window.ui

RESOURCES += \
    images.qrc \
    sounds.qrc
