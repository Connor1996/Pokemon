TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


HEADERS += \
    socket.h

Client{
    SOURCES += \
        client/main.cpp
}

Server{
    SOURCES += \
        server/main.cpp
}

