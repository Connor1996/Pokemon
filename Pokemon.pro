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

SOURCES += src/main.cpp\
        src/widget.cpp \
    src/pokemon.cpp \
    src/unittest.cpp \
    src/pokemonfactory.cpp \
    include/sqlite3.c \
    src/sqlconnector.cpp

HEADERS  += src/widget.h \
    src/pokemon.h \
    src/pokemonfactory.h \
    include/catch.hpp \
    include/sqlite3.h \
    src/sqlconnector.h



FORMS    += widget.ui

debug {
  DEFINES += __DEBUG__
}

