#include "widget.h"
#include <QApplication>
#include <QTextCodec>

#include <iostream>

//#if _MSC_VER >= 1600
//#pragma execution_character_set("UTF-8")
//#endif

int main(int argc, char *argv[])
{
   // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}


