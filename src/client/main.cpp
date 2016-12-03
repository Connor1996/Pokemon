#include "widget.h"
#include <QApplication>
#include <QTextCodec>

#include <iostream>


int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}


