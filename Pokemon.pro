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
    include/sqlite3.c




HEADERS  +=  \
    src/pokemon.h \
    include/sqlite3.h \
    src/reflector.h \
    src/socket.h \
    src/model/userinfo.h \
    include/json.hpp \
    src/define.h \
    src/model/userbag.h \
    src/model/pokemoninfo.h \
    src/model/pokemonlist.h



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
        src/client/gamelobby.cpp \
        src/pokemonfactory.cpp \
        src/client/userlistwidget.cpp \
        src/client/stackwidget.cpp \
        src/client/bagwidget.cpp \
        src/client/fightwidget.cpp \
        src/client/fightroom.cpp \
        src/client/scratchwidget.cpp


    HEADERS += \
        src/client/widget.h \
        src/client/client.h \
        src/client/gamelobby.h \
        src/pokemonfactory.h \
        src/client/userlistwidget.h \
        src/client/stackwidget.h \
        src/client/bagwidget.h \
        src/client/fightwidget.h \
        src/client/fightroom.h \
        src/client/scratchwidget.h

    FORMS    += \
        widget.ui \
        gamelobby.ui \
        src/client/userlistwidget.ui \
        src/client/stackwidget.ui \
        src/client/bagwidget.ui \
        src/client/fightwidget.ui \
        src/client/fightroom.ui \
        src/client/scratchwidget.ui
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





