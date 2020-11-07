QT -= gui
QT += network
QT += multimedia
QT += core
QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = ../../Server/executable/Pacman_Server

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    data_packet.cpp \
    map.cpp \
    movable_character.cpp \
    pacman.cpp \
    ghost.cpp \
    pacman_server.cpp \
    ../common/foodball.cpp \
    ../common/powerball.cpp

HEADERS += \
    data_packet.h \
    map.h \
    movable_character.h \
    pacman.h \
    ghost.h \
    pacman_server.h \
    ../common/common.h \
    ../common/foodball.h \
    ../common/powerball.h
