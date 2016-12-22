#include "widget.h"
#include <QApplication>
#include <QTextCodec>

#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}


