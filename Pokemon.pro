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
    src/reflector.cpp


HEADERS  +=  \
    src/pokemon.h \
    src/pokemonfactory.h \
    include/sqlite3.h \
    src/reflector.h \
    src/model.h \
    src/socket.h \
    src/model/userinfo.h \
    include/json.hpp \
    src/define.h


FORMS    += \
    widget.ui \
    gamelobby.ui

RESOURCES += \
    images.qrc


debug {
  DEFINES += __DEBUG__
}

Client{
    UI_DIR = ./ui

    DEFINES += __CLIENT__

    SOURCES += \
        src/client/main.cpp \
        src/client/widget.cpp  \
        src/client/client.cpp \
        src/client/gamelobby.cpp


    HEADERS += \
        src/client/widget.h \
        src/client/client.h \
        src/client/gamelobby.h
}

Server{
    DEFINES += __SERVER__

    SOURCES += \
        src/server/main.cpp \
        src/server/server.cpp \
        src/server/dispatch.cpp \
        src/unittest.cpp

    HEADERS += \
        include/catch.hpp \
        src/server/server.h \
        src/server/ormlite.h \
        src/server/dispatch.h

}



