#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T12:55:55
#
#-------------------------------------------------

Client {
    QT       += core gui
    CONFIG += c++14
}

Server {
    CONFIG += console c++14
    CONFIG -= app_bundle
    CONFIG -= qt
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pokemon
TEMPLATE = app


DEFINES += WIN32_LEAN_AND_MEAN  #ÅÅ³ýwindows.hÓëwinsock.hµÄ³å
SOURCES += \
    src/pokemon.cpp \
    src/pokemonfactory.cpp \
    include/sqlite3.c \
    src/sqlconnector.cpp \
    src/reflector.cpp


HEADERS  +=  \
    src/pokemon.h \
    src/pokemonfactory.h \
    include/sqlite3.h \
    src/sqlconnector.h \
    src/reflector.h \
    src/ormlite.h \
    src/model.h \
    src/socket.h


UI_DIR = ./ui

FORMS    += \
    widget.ui

debug {
  DEFINES += __DEBUG__
}

Client{
    SOURCES += \
        src/client/main.cpp \
        src/widget.cpp  \
        src/unittest.cpp \
        src/client/client.cpp


    HEADERS += \
        src/widget.h \
        include/catch.hpp \
        src/client/client.h

}

Server{
    SOURCES += \
        src/server/main.cpp \
        src/server/server.cpp
    HEADERS += \
        src/server/server.h

}


