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

SOURCES += main.cpp\
        widget.cpp \
    pokemon.cpp \
    unittest.cpp \
    pokemonfactory.cpp

HEADERS  += widget.h \
    pokemon.h \
    catch.hpp \
    pokemonfactory.h

FORMS    += widget.ui

debug {
  DEFINES += __DEBUG__
}

