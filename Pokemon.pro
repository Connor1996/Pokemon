#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T12:55:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pokemon
TEMPLATE = app
CONFIG += c++11

DEFINES += WIN32_LEAN_AND_MEAN  #排除windows.h与winsock.h的冲突

SOURCES +=\
        src/widget.cpp \
    src/pokemon.cpp \
    src/unittest.cpp \
    src/pokemonfactory.cpp \
    include/sqlite3.c \
    src/sqlconnector.cpp \
    src/reflector.cpp

HEADERS  += src/widget.h \
    src/pokemon.h \
    src/pokemonfactory.h \
    include/catch.hpp \
    include/sqlite3.h \
    src/sqlconnector.h \
    src/reflector.h \
    src/ormlite.h \
    src/model.h \
    src/socket.h


FORMS    += \
    widget.ui

debug {
  DEFINES += __DEBUG__
}

Client{
    SOURCES += \
        src/client/main.cpp
}

Server{
    SOURCES += \
        src/server/main.cpp
}


