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

CONFIG += c++17

TARGET = ../../Client/executable/Pacman_Client
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
    main.cpp \
    connection_dialog_window.cpp \
    game_window.cpp \
    ghost.cpp \
    pacman.cpp \
    screen_text_display.cpp \
    server_connection.cpp \
    sounds.cpp \
    status_bar_manager.cpp \
    ../common/map.cpp \
    ../common/foodball_manager.cpp \
    ../common/powerball_manager.cpp \
    ../common/log_manager.cpp \
    ../common/data_packet.cpp

HEADERS += \
    connection_dialog_window.h \
    game_window.h \
    ghost.h \
    movable_character.h \
    pacman.h \
    screen_text_display.h \
    server_connection.h \
    sounds.h \
    status_bar_manager.h \
    ../common/common.h \
    ../common/map.h \
    ../common/foodball_manager.h \
    ../common/powerball_manager.h \
    ../common/log_manager.h \
    ../common/data_packet.h

FORMS += \
    connection_dialog_window.ui \
    game_window.ui

RESOURCES += \
    images.qrc \
    sounds.qrc
